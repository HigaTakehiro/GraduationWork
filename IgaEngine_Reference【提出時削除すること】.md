# IgaEngine_リファレンス

## 目次
1. フォルダ構造
    - ApplicationCode
    - Engine
    - External
1. エンジン機能
    - Audio
        - Soundクラス
        - SoundManagerクラス
    - Base
        - DirectXSettingクラス
        - Easingクラス
        - ExternalFileLoaderクラス
        - JsonLoaderクラス
        - Quaternionクラス
        - SafeDeleteクラス
        - Vector2クラス
        - Vector3クラス
        - WinAppクラス
    - Camera
        - Cameraクラス
    - FBX
        - FbxLoaderクラス
        - FBXModelクラス
        - FBXObject3dクラス
    - Input
        - KeyInputクラス
        - MouseInputクラス
        - PadInputクラス
    - Light
        - LightGroupクラス
            - CircleShadow
            - DirectionalLight
            - PointLight
            - SpotLight
    - Object2d
        - DebugTextクラス
        - ImageManagerクラス
        - PostEffectクラス
        - Spriteクラス
        - TextDrawクラス
    - Object3d
        - MapChipクラス
        - Modelクラス
        - ModelManagerクラス
        - Object3dクラス
        - ParticleManagerクラス
        - Shapesクラス
    - Resource
        - 3DModels
            - fbx
            - obj
        - GameData
        - Images
        - Mapchip
        - shaders
            - Object2d
            - Object3d
                - fbx
                - obj
            - Particle
            - PostEffect
        - Sound
            - BGM
            - SE

***

## フォルダ構造
ここではこのエンジンがどのようなフォルダ構造になっているか
ざっくりと解説していく。

- ApplicationCode
ApplicationCodeフォルダ内ではゲームに関係するフォルダ及びファイルを
まとめている。ゲームに関わるコードはこのフォルダ内に入れていくこと。

- Engine
Engineフォルダ内ではエンジンに関するフォルダ及びファイルをまとめている。
Engine機能に関するコードはこのフォルダに追加すること。
また、IgaEngine製作者以外は基本的にこのフォルダにファイルを追加しないこと。
(追加する場合はIgaEngine製作者に連絡し、このマークダウンに解説を記述すること)

- External
Externalフォルダ内では外部からダウンロードしてきたライブラリ等をまとめている。
このフォルダ内のコードは個別のリファレンスを参照すること。

***

## エンジン機能
ここではEngineフォルダ内の各クラス及びその機能の解説をしていく。

- Audioフォルダ
ここでは音に関するファイルをまとめている。

- Soundクラス
XAudioを用いたサウンド再生を行うクラス
ただしサウンドを再生したい場合、SoundManagerクラスから再生するため
各シーンでこのクラスのインスタンスを生成することはない。
(main.cppでSoundクラスの初期化を行っているので改めて生成する必要はない)

    - Initialize()
    初期化を行う関数。
    Soundクラスを使用する際は必ず呼び出すこと。

    - PlaySoundData(const SoundData& soundData, bool isRoop, float volume)
    音を再生する関数。
    再生する時は音データをまとめた構造体とループするかどうかのフラグと
    音量を渡すことで再生できる。
    (ただし音を再生したい時はSoundManagerクラスから再生するのでこの関数を
    呼び出すことは基本的にはない)

    - StopSoundData(const SoundData& soundData, bool isPause)
    音を停止する関数。
    停止するときは音データをまとめた構造体と最初から再生するかどうかのフラグを渡すことで停止できる。
    (ただし音を停止したい時はSoundManagerクラスから停止するのでこの関数を
    呼び出すことは基本的にはない)

    - LoadSound(const std::string& fileName, SoundData& soundData)
    音データを読み込む関数
    読み込む際は読み込む音データと格納する音データをまとめた構造体を
    渡すことで構造体に音データが読み込まれる。
    (ただし音データを読み込む時はSoundManagerクラスから読み込むため
    この関数を呼び出すことは基本的にない)

- SoundManagerクラス
音データの管理、再生、停止、読み込みを行うクラス。
シングルトンパターンで呼び出せるためインスタンスを生成する必要はない。

    - BGMKey
    std::mapによりBGMを管理しているため、それと対応したキーを宣言するための
    列挙型。
    BGMを読み込む際には後述するInitialize(Sound* sound)関数での読み込みと
    この列挙型にキーを宣言する必要がある。

    - SEKey
    std::mapによりSEを管理しているため、それと対応したキーを宣言するための
    列挙型。
    SEを読み込む際には後述するInitialize(Sound* sound)関数での読み込みと
    この列挙型にキーを宣言する必要がある。

    - Initialize(Sound* sound)
    SoundManagerクラスの初期化を行うクラス。
    BGMとSEを読み込む際にはこの関数内で
    LoadBGM(const std::string& fileName, BGMKey bgmKey)及び
    LoadSE(const std::string& fileName, SEKey seKey)を呼び出す必要がある。
    その際に前述のBGMKey及びSEKeyの2つの列挙型にキーを宣言する必要がある。
    また、引数のfileNameには階層構造も記述する必要がある。
    音データはEngine/Resources/Soundフォルダ内の
    BGMならBGMフォルダに、SEならSEフォルダに格納する必要がある。
    対応している拡張子は.wavのみ。

    例:
    ```
    enum class BGMKey {
        Title
    };

    Initialize(Sound* sound) {
        sound_ = sound;
        LoadBGM("Engine/Resources/Sound/BGM/title.wav", SoundManager::BGMKey::Title);
    }
    ```

    - PlayBGM(const BGMKey bgmKey, const bool isRoop, const float volume)
    BGMの再生を行う関数。
    再生する際は、再生したいBGMのキー、ループ再生するかどうかを決めるフラグ、
    BGMのボリュームを渡すことで再生できる。
    ボリュームは0~1の範囲をとる。またサウンドがすでに再生されている場合は二重に再生されない。

    例:
    ```
    void TitleScene::Update() {
        SoundManager::GetIns()->PlayBGM(SoundManager::BGMKey::Title, true, 0.5f);
    }
    ```

    - PlaySE(const SEKey seKey, const float volume)
    SEの再生を行う関数。
    再生する際は、再生したいSEのキー、SEのボリュームを渡すことで再生できる。
    ボリュームは0~1の範囲をとる。

    例:
    ```
    void TitleScene::Update() {
        if (isJump_) {
            SoundManager::GetIns()->PlayBGM(SoundManager::BGMKey::Title, true, 0.5f);
        }
    }
    ```
    
    - StopBGM(const BGMKey bgmKey, const bool isPause)
    BGMの停止を行う関数。
    停止する際は、停止したいBGMのキーを渡すことで停止できる。
    また第二引数に一時停止するかどうかのフラグを渡すことで、
    BGMを一時停止するかどうかを決めることができる。

    例:
    ```
    void TitleScene::Update() {
        if (isPause_) {
            SoundManager::GetIns()->StopBGM(SoundManager::BGMKey::Title);
        }
    }
    ```

    - StopSE(const SEKey seKey, const bool isPause)
    SEの停止を行う関数。
    停止する際は、停止したいSEのキーを渡すことで停止できる。
    また第二引数に一時停止するかどうかのフラグを渡すことで、
    SEを一時停止するかどうかを決めることができる。

    例:
    ```
    void TitleScene::Update() {
        if (isPause_) {
            SoundManager::GetIns()->StopSE(SoundManager::BGMKey::Title, true, 0.5f);
        }
    }
    ```

    - LoadBGM(const std::string& fileName, BGMKey bgmKey)
    BGMを追加する関数。
    追加する際はファイルパスとBGMマップのキーを渡すことで追加することができる。
    使用例はInitialize(Sound* sound)を参照。
    追加する拡張子は.wavのみ。

    - LoadSE(const std::string& fileName, SEKey seKey)
    SEを追加する関数。
    追加する際はファイルパスとSEマップのキーを渡すことで追加することができる。
    追加する拡張子は.wavのみ。
    
    例:
    ```
    enum class SEKey {
        Jump
    };

    void Initialize(Sound* sound) {
        sound_ = sound;
        LoadSE("Engine/Resources/Sound/SE/jump.wav", SoundManager::SEKey::Jump);
    }
    ```

- DirectXSettingクラス
DirectXの設定を行うクラス。
デバイスの取得やコマンドリストの取得など
DirectXの設定に関わるものを取得する際に必要。
シングルトンパターンになっているためインスタンスを生成する必要はない。

    - Initialize(WinApp* win)
    DirectXの初期化を行う関数。
    使用する際はウィンドウの設定を行うクラスのインスタンスを渡す。
    ただし初期化はすでにされているため改めて呼ぶ必要はない。

    - PreDraw(XMFLOAT4 color)
    描画を行う際に必要になる前処理を行う関数。
    使用する際は背景色を渡す必要がある。
    描画する際はこの関数と後述のPostDraw()、beginDrawWithDirect2D()、
    endDrawWithDirect2D()と一緒に呼び出す必要がある。
    
    例:
    ```
    void TitleScene::Draw() {
        const XMFLOAT4 backColor = {0.2f, 0.2f, 0.6f, 1.0f};

        DirectXSetting::GetIns()->beginDrawWithDirect2D();
        //テキスト描画範囲
        D2D1_RECT_F textDrawRange = {0, 0, 500, 500};

        //文字描画処理

        DirectXSetting::GetIns()->endDrawWithDirect2D();

        DirectXSetting::GetIns()->PreDraw(backColor);
        //ポストエフェクトを掛けた描画処理
        //ポストエフェクトを掛けたくない描画処理
        DirectXSetting::GetIns()->PostDraw();
    }
    ```

    - PostDraw()
    描画を行った後の処理を行う関数。
    描画する際はこの関数と後述のPostDraw()、beginDrawWithDirect2D()、
    endDrawWithDirect2D()と一緒に呼び出す必要がある。

    コード例はPreDraw(XMFLOAT4 color)を参照。

    - GetDev()
    デバイスの取得を行う関数。
    DirectXのデバイスを取得したい際に呼び出す関数。

    例:
    ```
    ID3D12Device* dev = DirectXSetting::GetIns()->GetDev();
    ```

    - GetCmdList()
    コマンドリストの取得を行う関数。
    コマンドリストを取得をしたい際に呼び出す関数。

    例:
    ```
    ID3D12GraphicsCommandList* cmdList = DirectXSetting::GetIns()->GetCmdList();
    ```

    - GetD2DDeviceContext()
    D2Dデバイス環境の取得を行う関数。
    Direct2Dを通した描画を行う際に必要なD2Dデバイス環境を取得したい際に必要になる。

    例:  
    ```
    ID2D1DeviceContext* d2dDev = DirectXSetting::GetIns()->GetD2DDeviceContext();
    ```

    - GetDWriteFactory()
    DirectWriteFactoryの取得を行う関数。
    DirectWriteの様々な設定が入っているDirectWriteFactoryを取得したい際に必要になる。

    例:
    ```
    IDWriteFactory* dWriteFactory = DirectXSetting::GetIns()->GetDWriteFactory();
    ```

    - GetColorBrushes()
    DirectWriteを用いた文字描画をする際に必要になる文字色をまとめたstd::mapを
    取得する際に必要になる関数。
    必要な色が欲しい際はstd::mapなので.at(string型のキー)で取得できる。
    ただし追加した色のキーを正しく入力しないと取得できない。
    
    例:
    ```
    //カラーブラシmapに白色を追加
    DirectXSetting::GetIns()->registerSolidColorBrush("white", D2D1::ColorF::White);

    //カラーブラシmapを取得
    std::map<std::string, ComPtr<ID2D1SolidColorBrush>> brushes = DirectXSetting::GetIns()->GetColorBrushes();

    //白色を取得
    const auto solidColorBrush = brushes.at("white");
    ```

    - registerSolidColorBrush(const std::string& key, const D2D1::ColorF color)
    カラーブラシ(文字色)の追加をする関数。
    追加する際には連想配列のキーと追加したい文字色(D2D1::ColorF)を渡すことで追加される。
    追加するときにはTextDrawクラスのInitialize()で行うこと。

    
    ```
    void TextDraw::Initialize() {
        DirectXSetting::GetIns()->registerSolidColorBrush("white", D2D1::ColorF::White);
    }
    ```

    - registerTextFormat(const std::string& key, const std::wstring& fontName, const float fontSize)
    DirectWriteのフォントを追加する関数。
    追加する際には連想配列のキーと追加したいフォントの名前、フォントの大きさを渡すことで追加される。
    追加するときにはTextDrawクラスのInitialize()で行うこと。

    
    ```
    void TextDraw::Initialize() {
        DirectXSetting::GetIns()->registerTextFormat("meiryo", L"メイリオ", 32);
    }
    ```

    - beginDrawWithDirect2D()
    Direct2Dを用いる描画処理を使用する際に呼び出す描画前処理。
    後述のendDrawWithDirect2D()内で画面をフリップする処理を行っているため必ず
    この関数とセットで呼び出すようにすること。

    コード例はPreDraw(XMFLOAT4 color)を参照すること。

    - endDrawWithDirect2D()
    Direct2Dを用いる描画処理を使用する際に呼び出す描画後処理。
    この関数内で画面をフリップする処理を行っているため、
    前述のbeginDrawWithDirect2D()とセットで呼び出すようにすること。

    コード例はPreDraw(XMFLOAT4 color)を参照すること。

- Easingクラス
イージングを用いた移動の計算を行うクラス。

    - easeIn(float time, float maxTime, float maxPos, float nowPos)
    イーズインによる補間を行う関数。
    利用する際は現在時間、掛ける時間、最終地点(1軸のみ)、現在座標(1軸のみ)を渡す必要がある。

    例:
    ```
    //座標
    Vector2 pos_ = {0.0f, 0.0f};
    //経過時間
    float time_ = 0.0f;
    
    //定数
    //移動時間
    const float maxTime = 120.0f;
    //最終地点
    const float lastPos = 500.0f;

    if (++time_ < maxTime) {
        pos_.x = Easing::easeIn(time_, maxtime, lastPos, pos_.x);
    }
    ```

    - easeOut(float time, float maxTime, float maxPos, float nowPos)
    イーズアウトによる補間を行う関数。
    利用する際は現在時間、掛ける時間、最終地点(1軸のみ)、現在座標(1軸のみ)を渡す必要がある。

    コード例はeaseIn()と同じなのでeaseIn()を参照。

    - easeInOut(float time, float maxTime, float maxPos, float nowPos)
    イーズインアウトによる補間を行う関数。
    利用する際は現在時間、掛ける時間、最終地点(1軸のみ)、現在座標(1軸のみ)を渡す必要がある。

    コード例はeaseIn()と同じなのでeaseIn()を参照。

    - easeInBack(float time, float maxTime, float maxPos, float nowPos, float back = 1.70158f)
    イーズインバックによる補間を行う関数。
    利用する際は現在時間、掛ける時間、最終地点(1軸のみ)、現在座標(1軸のみ)、バック定数

