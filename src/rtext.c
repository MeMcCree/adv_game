Font GetFontDefault(void);
Font LoadFont(const char *fileName);
Font LoadFontEx(const char *fileName, int fontSize, int *codepoints, int codepointCount);
Font LoadFontFromImage(Image image, Color key, int firstChar);
Font LoadFontFromMemory(const char *fileType, const unsigned char *fileData, int dataSize, int fontSize, int *codepoints, int codepointCount);
bool IsFontValid(Font font);
GlyphInfo *LoadFontData(const unsigned char *fileData, int dataSize, int fontSize, int *codepoints, int codepointCount, int type);
Image GenImageFontAtlas(const GlyphInfo *glyphs, Rectangle **glyphRecs, int glyphCount, int fontSize, int padding, int packMethod);
void UnloadFontData(GlyphInfo *glyphs, int glyphCount);
void UnloadFont(Font font);
bool ExportFontAsCode(Font font, const char *fileName);

void DrawFPS(int posX, int posY);
void DrawText(const char *text, int posX, int posY, int fontSize, Color color);
void DrawTextEx(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint);
void DrawTextPro(Font font, const char *text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint);
void DrawTextCodepoint(Font font, int codepoint, Vector2 position, float fontSize, Color tint);
void DrawTextCodepoints(Font font, const int *codepoints, int codepointCount, Vector2 position, float fontSize, float spacing, Color tint);

void SetTextLineSpacing(int spacing);
int MeasureText(const char *text, int fontSize);
Vector2 MeasureTextEx(Font font, const char *text, float fontSize, float spacing);
int GetGlyphIndex(Font font, int codepoint);
GlyphInfo GetGlyphInfo(Font font, int codepoint);
Rectangle GetGlyphAtlasRec(Font font, int codepoint);

char *LoadUTF8(const int *codepoints, int length);
void UnloadUTF8(char *text);
int *LoadCodepoints(const char *text, int *count);
void UnloadCodepoints(int *codepoints);
int GetCodepointCount(const char *text);
int GetCodepoint(const char *text, int *codepointSize);
int GetCodepointNext(const char *text, int *codepointSize);
int GetCodepointPrevious(const char *text, int *codepointSize);
const char *CodepointToUTF8(int codepoint, int *utf8Size);

int TextCopy(char *dst, const char *src);
bool TextIsEqual(const char *text1, const char *text2);
unsigned int TextLength(const char *text);
const char *TextFormat(const char *text, ...);
const char *TextSubtext(const char *text, int position, int length);
char *TextReplace(const char *text, const char *replace, const char *by);
char *TextInsert(const char *text, const char *insert, int position);
const char *TextJoin(const char **textList, int count, const char *delimiter);
const char **TextSplit(const char *text, char delimiter, int *count);
void TextAppend(char *text, const char *append, int *position);
int TextFindIndex(const char *text, const char *find);
const char *TextToUpper(const char *text);
const char *TextToLower(const char *text);
const char *TextToPascal(const char *text);
const char *TextToSnake(const char *text);
const char *TextToCamel(const char *text);

int TextToInteger(const char *text);
float TextToFloat(const char *text);