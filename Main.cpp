
# include <Siv3D.hpp> // OpenSiv3D v0.6
# include <Siv3D/OpenCV_Bridge.hpp>
# include <opencv2/opencv.hpp>
# include <opencv2/text.hpp>




SIV3D_SET(EngineOption::Renderer::Direct3D11)
//SIV3D_SET(EngineOption::Renderer::OpenGL)
//SIV3D_SET(EngineOption::D3D11Driver::WARP)

struct Result
{
	s3d::String st;
	s3d::Rect re;
	double probability = 0.0;
};

Array<Result> RecognizeCharacterFromImage(Image handwritten)
{
	Array<Result> results;
	Result result;
	// 画像読み込み
	// const s3d::Image handwritten(U"example/hand.png");
	cv::Mat image = s3d::OpenCV_Bridge::ToMatVec3bBGR(handwritten);
	// cv::Mat image = cv::imread("example\\hand.png");
	// Print << image.size().width;

	// グレースケール化
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);
	// 文字認識クラスのインスタンス生成
	auto ocr = cv::text::OCRTesseract::create("tessdata\\", "jpn");
	
	// ホワイトリストを消す
	ocr->setWhiteList("");

	std::string text;
	std::vector<cv::Rect> boxes;
	std::vector<std::string> words;
	std::vector<float> confidences;
	// 文字認識の実行
	ocr->run(gray, text, &boxes, &words, &confidences);

	// 結果出力
	// 文字のかたまりごとに出力
	printf(" 文字      | 位置       | 大きさ     | 信頼度\n");
	printf("-----------+------------+------------+----------\n");
	for (int i = 0; i < boxes.size(); i++)
	{
		printf("%-10s | (%3d, %3d) | (%3d, %3d) | %f\n",
			words[i].c_str(),
			boxes[i].x, boxes[i].y,
			boxes[i].width, boxes[i].height,
			confidences[i]);
		result.st = Unicode::FromUTF8(words[i]);
		result.re.x = boxes[i].x, result.re.y = boxes[i].y;
		result.re.w = boxes[i].width, result.re.h = boxes[i].height;
		result.probability = confidences[i];
		results.push_back(result);
	}
	String s;
	if (boxes.size() > 0) 
	{
		// 文字コードを変換
		s = Unicode::FromUTF8(text);
	}
	Print << s;
	return results;
}



void Main()
{
	// {
		// Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });
		Scene::SetBackground(Palette::Black);
		Scene::SetResizeMode(ResizeMode::Keep);
		Window::SetStyle(WindowStyle::Sizable);
		// Window::Resize(1280, 720);

		// 画像読み込み
		const s3d::Image handwritten(U"Screenshot/hand.png");
		/**/
		cv::Mat image = s3d::OpenCV_Bridge::ToMatVec3bBGR(handwritten);
		// cv::Mat image = cv::imread("example\\hand.png");
		// Print << image.size().width;

		// グレースケール化
		cv::Mat gray;
		cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);
		// 文字認識クラスのインスタンス生成
		auto ocr = cv::text::OCRTesseract::create("tessdata\\", "jpn");

		// ホワイトリストを消す
		ocr->setWhiteList("");

		std::string text;
		std::vector<cv::Rect> boxes;
		std::vector<std::string> words;
		std::vector<float> confidences;
	{
		// 文字認識の実行
		ocr->run(gray, text, &boxes, &words, &confidences);

		// 結果出力

		
		// 文字のかたまりごとに出力
		// printf(" 文字      | 位置       | 大きさ     | 信頼度\n");
		// printf("-----------+------------+------------+----------\n");
		for (int i = 0; i < boxes.size(); i++)
		{
			printf("%-10s | (%3d, %3d) | (%3d, %3d) | %f\n",
				words[i].c_str(),
				boxes[i].x, boxes[i].y,
				boxes[i].width, boxes[i].height,
				confidences[i]);
		}
		String s = U"hello";
		if (boxes.size() > 0) {
			// 文字コードを変換
			s = Unicode::FromUTF8(text);
		}
		Print << s;
		
	}
	/**/
	// RecognizeCharacterFromImage(handwritten);


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
		}

		// テクスチャを表示
		texture.draw();
	}
}

