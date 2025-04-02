PInt = ^Integer;

TTraceLogCallback = procedure (logLevel: Integer; text: PChar; va_args: array of TVarRec);
TLoadFileDataCallback = function (fileName: PChar; dataSize: PInt): PByte;
TSaveFileDataCallback = function (fileName: PChar; data: PByte; dataSize: Integer): Boolean;
TLoadFileTextCallback = function (fileName: PChar): PChar;
TSaveFileTextCallback = function (fileName: PChar; text: PChar): Boolean;

TVector2 = packed record
    x, y: Single;
end;
PTVector2 = ^TVector2;
TVector3 = packed record
    x, y, z: Single;
end;
PTVector3 = ^TVector3;
TVector4 = packed record
    x, y, z, w: Single;
end;
PTVector4 = ^TVector4;
TQuaternion = TVector4;
PTQuaternion = ^TQuaternion;

TMatrix = packed record
    m0, m4, m8, m12: Single;
    m1, m5, m9, m13: Single;
    m2, m6, m10, m14: Single;
    m3, m7, m11, m15: Single;
end;
PTMatrix = ^TMatrix;
TColor = packed record
    r, g, b, a: Byte;
end;
PTColor = ^TColor;
TRectangle = packed record
    x, y, width, height: Single;
end;
PTRectangle = ^TRectangle;
PPTRectangle = ^PTRectangle;

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
TTextureCubemap = TTexture;
PTTextureCubemap = ^TTextureCubemap;

TRenderTexture = packed record
    id: dWord;
    texture: TTexture;
    depth: TTexture;
end;
PTRenderTexture = ^TRenderTexture;
TRenderTexture2D = TRenderTexture;
PTRenderTexture2D = ^TRenderTexture2D;

TNPatchInfo = packed record
    source: TRectangle;
    left: Integer;
    top: Integer;
    right: Integer;
    bottom: Integer;
    layout: Integer;
end;
PTNPatchInfo = ^TNPatchInfo;
TGlyphInfo = packed record
    v: Integer;
    offsetX: Integer;
    offsetY: Integer;
    advanceX: Integer;
    image: TImage;
end;
PTGlyphInfo = ^TGlyphInfo;
TFont = packed record
    baseSize: Integer;
    glyphCount: Integer;
    glyphPadding: Integer;
    texture: TTexture2D;
    recs: PTRectangle;
    glyphs: PTGlyphInfo;
end;
PTFont = ^TFont;

TCamera2D = packed record
    offset: TVector2;
    target: TVector2;
    rotation: Single;
    zoom: Single;
end;
PTCamera2D = ^TCamera2D;
TCamera3D = packed record
    position: TVector3;
    target: TVector3;
    up: TVector3;
    fovy: Single;
    projection: Integer;
end;
PTCamera3D = ^TCamera3D;
TCamera = TCamera3D;
PTCamera = ^TCamera;

TShader = packed record
    id: dWord;
    locs: PInt;
end;
PTShader = ^TShader;
TMaterialMap = packed record
    texture: TTexture2D;
    color: TColor;
    v: Single;
end;
PTMaterialMap = ^TMaterialMap;
TMaterial = packed record
    shader: TShader;
    maps: PTMaterialMap;
    params: Array[0..3] of Single;
end;
PTMaterial = ^TMaterial;
TMesh = packed record
    vertexCount: Integer;
    triangleCount: Integer;

    vertices: PSingle;
    texcoords: PSingle;
    texcoords2: PSingle;
    normals: PSingle;
    tangents: PSingle;
    colors: PByte;
    indices: Pword;

    animVertices: PSingle;
    animNormals: PSingle;
    boneIds: PByte;
    boneWeights: PSingle;
    boneMatrices: PTMatrix;
    boneCount: Integer;

    vaoId: dWord;
    vboId: PdWord;
end;
PTMesh = ^TMesh;

TBoneInfo = packed record
    name: Array[0..31] of Char;
    parent: Integer;
end;
PTBoneInfo = ^TBoneInfo;

TTransform = packed record
    translation: TVector3;
    rotation: TQuaternion;
    scale: TVector3;
end;
PTTransform = ^TTransform;
PPTTransform = ^PTTransform;

TModel = packed record
    transform: TMatrix;

    meshCount: Integer;
    materialCount: Integer;
    meshes: PTMesh;
    materials: PTMaterial;
    meshMaterial: PInt;

    boneCount: Integer;
    bones: PTBoneInfo;
    bindPose: PTTransform;
end;
PTModel = ^TModel;
TModelAnimation = packed record
    boneCount: Integer;
    frameCount: Integer;
    bones: PTBoneInfo;
    framePoses: PPTTransform;
    name: Array[0..31] of Char;
end;
PTModelAnimation = ^TModelAnimation;
TRay = packed record
    position: TVector3;
    direction: TVector3;
end;
PTRay = ^TRay;
TRayCollision = packed record
    hit: Boolean;
    distance: Single;
    point: TVector3;
    normal: TVector3;
end;
PTRayCollision = ^TRayCollision;
TBoundingBox = packed record
    min: TVector3;
    max: TVector3;
end;
PTBoundingBox = ^TBoundingBox;

TWave = packed record
    frameCount: dWord;
    sampleRate: dWord;
    sampleSize: dWord;
    channels: dWord;
    data: PByte;
end;
PTWave = ^TWave;
PTrAudioBuffer = ^Byte;
PTrAudioProcessor = ^Byte;
TAudioStream = packed record
    buffer: PTrAudioBuffer;
    processor: PTrAudioProcessor;

    sampleRate: dWord;
    sampleSize: dWord;
    channels: dWord;
end;
PTAudioStream = ^TAudioStream;
TSound = packed record
    stream: TAudioStream;
    frameCount: dWord;
end;
PTSound = ^TSound;
TMusic = packed record
    stream: TAudioStream;
    frameCount: dWord;
    looping: Boolean;

    ctxType: Integer;
    ctxData: PByte;
end;
PTMusic = ^TMusic;
TAudioCallback = procedure (bufferData: PByte; frames: dWord);

TVrDeviceInfo = packed record
    hResolution: Integer;
    vResolution: Integer;
    hScreenSize: Single;
    vScreenSize: Single;
    eyeToScreenDistance: Single;
    lensSeparationDistance: Single;
    interpupillaryDistance: Single;
    lensDistortionValues: Array[0..3] of Single;  
    chromaAbCorrection: Array[0..3] of Single;    
end;
PTVrDeviceInfo = ^TVrDeviceInfo;
TVrStereoConfig = packed record
    projection: Array[0..1] of TMatrix;
    viewOffset: Array[0..1] of TMatrix;
    leftLensCenter: Array[0..1] of Single;
    rightLensCenter: Array[0..1] of Single;
    leftScreenCenter: Array[0..1] of Single;
    rightScreenCenter: Array[0..1] of Single;
    scale: Array[0..1] of Single;
    scaleIn: Array[0..1] of Single;
end;
PTVrStereoConfig = ^TVrStereoConfig;

TFilePathList = packed record
    capacity: dWord;
    count: dWord;
    paths: PPChar;
end;
PTFilePathList = ^TFilePathList;

// NOTE: type replaced with mtype
TAutomationEvent = packed record
    frame: dWord;
    mtype: dWord;
    params: Array[0..3] of Integer;                  
end;
PTAutomationEvent = ^TAutomationEvent;
TAutomationEventList = packed record
    capacity: dWord;
    count: dWord;
    events: PTAutomationEvent;
end;
PTAutomationEventList = ^TAutomationEventList;