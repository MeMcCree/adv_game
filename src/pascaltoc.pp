{$mode objfpc}{$H+}

program clex;
type
	TTokenType = (TK_NAME, TK_LBRACE, TK_RBRACE, TK_COMMA);
	TToken = record
		typ: TTokenType;
		znacz: ShortString;
	end;
	TParam = record
		typ: String;
		znacz: String;
	end;
	TLexer = record
		data: ^char;
		dataLen: integer;
		p: integer;
	end;
const
	TkTypeNames: Array[Low(TTokenType)..High(TTokenType)] of String = (
		'TK_NAME',
		'TK_LBRACE',
		'TK_RBRACE',
		'TK_COMMA'
	);

function LexerReadLn(Var lexer: TLexer): String;
var
	line: String;
	len: Integer;
begin
	if lexer.p >= lexer.dataLen then
	begin
		exit('#0');
	end;
	len := 0;
	while (len < lexer.dataLen) and (Byte(lexer.data[lexer.p + len]) <> 10) do
	begin
		len += 1;
	end;
	if len = 0 then
		exit('');
	SetLength(line, len);
	Move((lexer.data + lexer.p)^, line[1], len);
	result := line;
	inc(lexer.p, len + 1);
end;

function LexerNext(Var lexer: TLexer): char;
begin
	if lexer.p >= lexer.dataLen then
	begin
		exit(chr(0));
	end;
	result := lexer.data[lexer.p];
	inc(lexer.p);
end;

function IsNameChar(c: char): Boolean;
begin
	case c of
		'a'..'z', 'A'..'Z', '0'..'9', '_', '*':
			result := true;
	else
		result := false;
	end;
end;

var
	path: String;
	fin: File of char;
	lexer: TLexer;
	lineLexer: TLexer;
	c, prc: char;
	Tok: TToken;
	braces: Boolean;
	line: String;
	lineTokens: array of TToken;
	lineTokensLen: Integer;
	i, l, r, paramStart: Integer;
	retType, funcName, pascParams: String;
	params: array of TParam;
	param: TParam;
	paramsLen: Integer;
begin
	if ParamCount < 1 then
	begin
		writeln('[ERROR] Expected filepath');
		ExitCode := 1;
		exit;
	end;

	path := paramStr(1);
	AssignFile(fin, path);
	try
		Reset(fin);
		lexer.dataLen := Filesize(fin);
		lexer.p := 0;

		lexer.data := Getmem(lexer.dataLen + 1);
		FillChar(lexer.data^, lexer.dataLen + 1, 0);
		BlockRead(fin, lexer.data^, lexer.dataLen);

		line := LexerReadLn(lexer);

		while line <> '#0' do
		begin
			if line = '' then
			begin
				writeln();
				c := LexerNext(lexer);
				if c = chr(0) then
				begin
					writeln('exiting');
					ExitCode := 0;
					exit;
				end;
				line := LexerReadLn(lexer);;
				Continue;
			end;

			lineLexer.data := @line[1];
			lineLexer.dataLen := Length(line);
			lineLexer.p := 0;

			SetLength(lineTokens, 0);
			lineTokensLen := 0;

			retType := '';
			funcName := '';
			pascParams := '';
			paramsLen := 0;
			paramStart := 0;
			SetLength(params, 0);
			braces := false;

			c := LexerNext(lineLexer);
			while c <> chr(0) do
			begin
				Case c of
					'a'..'z', 'A'..'Z', '_', '*':
					begin
						Tok.typ := TK_NAME;
						Tok.znacz := '';
						prc := '#';
						while IsNameChar(c) and (prc <> '*') do
						begin
							Tok.znacz := Tok.znacz + c;
							prc := c;
							c := LexerNext(lineLexer);
						end;

						SetLength(lineTokens, lineTokensLen + 1);
						lineTokens[lineTokensLen] := Tok;
						inc(lineTokensLen);
					end;
					'(':
					begin
						braces := true;
						Tok.typ := TK_LBRACE;
						Tok.znacz := '';
						c := LexerNext(lineLexer);
						SetLength(lineTokens, lineTokensLen + 1);
						lineTokens[lineTokensLen] := Tok;
						inc(lineTokensLen);
						paramStart := lineTokensLen;
					end;
					')':
					begin
						braces := false;
						Tok.typ := TK_RBRACE;
						Tok.znacz := '';
						c := LexerNext(lineLexer);

						if (lineTokensLen - paramStart) > 1 then
						begin
							param.typ := '';
							param.znacz := '';

							for i := paramStart to lineTokensLen-2 do
							begin
								if i = paramStart then
								begin
									param.typ := lineTokens[i].znacz;
								end
								else
								begin
									param.typ := param.typ + ' ' + lineTokens[i].znacz;
								end;
							end;
							param.znacz := lineTokens[lineTokensLen-1].znacz;

							SetLength(params, paramsLen + 1);
							params[paramsLen] := param;
							inc(paramsLen);
						end;

						SetLength(lineTokens, lineTokensLen + 1);
						lineTokens[lineTokensLen] := Tok;
						inc(lineTokensLen);
					end;
					',':
					begin
						Tok.typ := TK_COMMA;
						Tok.znacz := '';
						c := LexerNext(lineLexer);

						if (lineTokensLen - paramStart) > 1 then
						begin
							param.typ := '';
							param.znacz := '';

							for i := paramStart to lineTokensLen-2 do
							begin
								if i = paramStart then
								begin
									param.typ := lineTokens[i].znacz;
								end
								else
								begin
									param.typ := param.typ + ' ' + lineTokens[i].znacz;
								end;
							end;
							param.znacz := lineTokens[lineTokensLen-1].znacz;

							SetLength(params, paramsLen + 1);
							params[paramsLen] := param;
							inc(paramsLen);

							paramStart := lineTokensLen + 1;
						end;

						SetLength(lineTokens, lineTokensLen + 1);
						lineTokens[lineTokensLen] := Tok;
						inc(lineTokensLen);
					end;
				else
					c := LexerNext(lineLexer);
				end;
			end;

			(*for i := 0 to paramsLen-1 do
			begin
				write('[', params[i].typ, ', ', params[i].znacz, '] ');
			end;
			writeln();

			for i := 0 to lineTokensLen-1 do
			begin
				write('[', TkTypeNames[lineTokens[i].typ], '] ');
			end;
			writeln();*)

			l := 0;
			while l < paramsLen do
			begin
				r := l + 1;
				if l <> 0 then
					pascParams := pascParams + '; ';
				pascParams := pascParams + params[l].znacz;
				while (r < paramsLen) and (params[r].typ = params[l].typ) do
				begin
					pascParams := pascParams + ', ' + params[r].znacz;
					inc(r);
				end;
				pascParams := pascParams + ': ' + params[l].typ;
				l := r;
			end;
			//writeln(pascParams);

			i := 0;
			while lineTokens[i + 1].typ <> TK_LBRACE do
			begin
				if i = 0 then
				begin
					retType := lineTokens[i].znacz;
				end
				else
				begin
					retType := retType + ' ' + lineTokens[i].znacz;
				end;
				i := i + 1;
			end;

			funcName := lineTokens[i].znacz;

			if retType = 'void' then
			begin
				writeln('procedure ', funcname, '(', pascParams, '); cdecl; external;');
			end
			else
			begin
				writeln('function ', funcname, '(', pascParams, '): ', retType, '; cdecl; external;');
			end;

			line := LexerReadLn(lexer);
		end; 

		Freemem(lexer.data);
	finally
		CloseFile(fin);
	end;
end.