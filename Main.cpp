
# include <Siv3D.hpp> // OpenSiv3D v0.6
# include <Siv3D/OpenCV_Bridge.hpp>
# include <opencv2/opencv.hpp>
# include <opencv2/text.hpp>




SIV3D_SET(EngineOption::Renderer::Direct3D11)
//SIV3D_SET(EngineOption::Renderer::OpenGL)
//SIV3D_SET(EngineOption::D3D11Driver::WARP)



s3d::String RecognizeCharacterFromImage(Image handwritten,
  std::string& text,
  std::vector<cv::Rect>& boxes,
  std::vector<std::string>& words,
  std::vector<float>& confidences)
{
  
  // 画像変換
  cv::Mat image = s3d::OpenCV_Bridge::ToMatVec3bBGR(handwritten);

  // グレースケール化
  cv::Mat gray;
  cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);

  {
    // 文字認識クラスのインスタンス生成
    cv::Ptr<cv::text::OCRTesseract> ocr = cv::text::OCRTesseract::create("tessdata", "jpn");
    // ホワイトリストを消す
    ocr->setWhiteList("");

    // 文字認識の実行
    ocr->run(gray, text, &boxes, &words, &confidences);
  }
  // 結果出力
  String s = U"error";
  if (boxes.size() > 0)
  {
    // 文字コードを変換
    s = Unicode::FromUTF8(text);
  }

  return s;
}



void Main()
{

  Scene::SetBackground(Palette::Black);
  Scene::SetResizeMode(ResizeMode::Keep);
  Window::SetStyle(WindowStyle::Sizable);

  // 認識結果を保存する
  std::string text;
  std::vector<cv::Rect> boxes;
  std::vector<std::string> words;
  std::vector<float> confidences;
  Array<Rect> results;
  
  
  AsyncTask<String> task;

  // 画像読み込み
  const s3d::Image handwritten(U"Screenshot/hand.png");


  // スケッチから文字認識

   // キャンバスのサイズ
  constexpr Size canvasSize(600, 600);

  // ペンの太さ
  constexpr int32 thickness = 8;

  // ペンの色
  constexpr Color penColor = Palette::Orange;

  // 書き込み用の画像データを用意
  Image dynamicImage(canvasSize, Palette::White);

  // 表示用のテクスチャ（内容を更新するので DynamicTexture）
  DynamicTexture texture(dynamicImage);


  while (System::Update())
  {
    if (MouseL.pressed())
    {
      // 書き込む線の始点は直前のフレームのマウスカーソル座標
      // （初回はタッチ操作時の座標のジャンプを防ぐため、現在のマウスカーソル座標にする）
      const Point from = MouseL.down() ? Cursor::Pos() : Cursor::PreviousPos();

      // 書き込む線の終点は現在のマウスカーソル座標
      const Point to = Cursor::Pos();

      // image に線を書き込む
      Line(from, to).overwrite(dynamicImage, thickness, penColor);

      // 書き込み終わった image でテクスチャを更新
      texture.fill(dynamicImage);
    }

    // 描いたものを消去するボタンが押されたら
    if (SimpleGUI::Button(U"Clear", Vec2(640, 40), 120))
    {
      // 画像を白で塗りつぶす
      dynamicImage.fill(Palette::White);

      // 塗りつぶし終わった image でテクスチャを更新
      texture.fill(dynamicImage);

      // 認識した場所の情報を消す
      results.clear();
    }

    if (SimpleGUI::Button(U"Recognize", Vec2(640, 100), 120))
    {
      task = CreateAsyncTask(RecognizeCharacterFromImage, std::ref(dynamicImage), std::ref(text), std::ref(boxes), std::ref(words), std::ref(confidences));
    }
    if (task.isReady())
    {
      Print << task.get();
      for (int i = 0; i < boxes.size(); i++)
      {
        Rect(boxes[i].x, boxes[i].y, boxes[i].width, boxes[i].height).drawFrame(1, 1, Palette::Orange);
        results.push_back(Rect(boxes[i].x, boxes[i].y, boxes[i].width, boxes[i].height));
      }
    }
    // テクスチャを表示
    texture.draw();
    for (const auto& result : results) 
    {
      result.drawFrame(1, 1, Palette::Orange);
    }
  }
}
