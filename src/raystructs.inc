TTraceLogCallback = packed record end;
PTTraceLogCallback = ^TTraceLogCallback;
TLoadFileDataCallback = packed record end;
PTLoadFileDataCallback = ^TLoadFileDataCallback;
TSaveFileDataCallback = packed record end;
PTSaveFileDataCallback = ^TSaveFileDataCallback;
TLoadFileTextCallback = packed record end;
PTLoadFileTextCallback = ^TLoadFileTextCallback;
TSaveFileTextCallback = packed record end;
PTSaveFileTextCallback = ^TSaveFileTextCallback;

TVector2 = packed record end;
PTVector2 = ^TVector2;
TVector3 = packed record end;
PTVector3 = ^TVector3;
TVector4 = packed record end;
PTVector4 = ^TVector4;
TMatrix = packed record end;
PTMatrix = ^TMatrix;
TColor = packed record
    r, g, b, a: Byte;
end;
PTColor = ^TColor;
TRectangle = packed record end;
PTRectangle = ^TRectangle;

TImage = packed record
    data: ^Byte;
    width: Integer;
    height: Integer;
    mipmaps: Integer;
    format: Integer;
end;
PTImage = ^TImage;
TTexture = packed record
    id: dWord;
    width: Integer;
    height: Integer;
    mipmaps: Integer;
    format: Integer;
end;
PTTexture = ^TTexture;
TTexture2D = TTexture;
PTTexture2D = ^TTexture2D;
TRenderTexture = packed record end;
PTRenderTexture = ^TRenderTexture;
TRenderTexture2D = TRenderTexture;
PTRenderTexture2D = ^TRenderTexture2D;
TNPatchInfo = packed record end;
PTNPatchInfo = ^TNPatchInfo;
TGlyphInfo = packed record end;
PTGlyphInfo = ^TGlyphInfo;
TFont = packed record end;
PTFont = ^TFont;
TTextureCubemap = packed record end;

TCamera2D = packed record end;
PTCamera2D = ^TCamera2D;
TCamera3D = packed record end;
PTCamera3D = ^TCamera3D;
TCamera = TCamera3D;
PTCamera = ^TCamera;

TShader = packed record end;
PTShader = ^TShader;
TMaterialMap = packed record end;
PTMaterialMap = ^TMaterialMap;
TMaterial = packed record end;
PTMaterial = ^TMaterial;
TMesh = packed record end;
PTMesh = ^TMesh;
TModel = packed record end;
PTModel = ^TModel;
TModelAnimation = packed record end;
PTModelAnimation = ^TModelAnimation;
TTransform = packed record end;
PTTransform = ^TTransform;
TBoneInfo = packed record end;
PTBoneInfo = ^TBoneInfo;
TRay = packed record end;
PTRay = ^TRay;
TRayCollision = packed record end;
PTRayCollision = ^TRayCollision;
TBoundingBox = packed record end;
PTBoundingBox = ^TBoundingBox;

TWave = packed record end;
PTWave = ^TWave;
TAudioStream = packed record end;
PTAudioStream = ^TAudioStream;
TSound = packed record end;
PTSound = ^TSound;
TMusic = packed record end;
PTMusic = ^TMusic;

TVrDeviceInfo = packed record end;
PTVrDeviceInfo = ^TVrDeviceInfo;
TVrStereoConfig = packed record end;
PTVrStereoConfig = ^TVrStereoConfig;

TFilePathList = packed record end;
PTFilePathList = ^TFilePathList;

TAutomationEvent = packed record end;
PTAutomationEvent = ^TAutomationEvent;
TAutomationEventList = packed record end;
PTAutomationEventList = ^TAutomationEventList;