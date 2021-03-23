
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

s3d::Array<Result> RecognizeCharacterFromImage(const s3d::Image handwritten)
{
	Array<Result> results;
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
	}
	String s = U"hello";
	if (boxes.size() > 0) {
		// 文字コードを変換
		s = Unicode::FromUTF8(text);
	}
	Print << s;

}



void Main()
{

	/*
	// Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });
	Scene::SetBackground(Palette::Black);
	Scene::SetResizeMode(ResizeMode::Keep);
	Window::SetStyle(WindowStyle::Sizable);
	// Window::Resize(1280, 720);

	// 画像読み込み
	const s3d::Image handwritten(U"example/hand.png");
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

	printf("%s\n", text.c_str());
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
	}
	String s = U"hello";
	if (boxes.size() > 0) {
		// 文字コードを変換
		s = Unicode::FromUTF8(text);
	}
	Print << s;
	*/
	
	
	while (System::Update())
	{
		// for (auto i : step(20))
		{
			// Rect{ Point{ 100, 100 } *i, 100 }.draw();
		}

		for (auto i : step(20))
		{
			//Rect{ Cursor::Pos().movedBy(0 + i * 20, 0), 20, 400 }
				//.draw(HSV{ i * 10.0, 0.5, 0.9 });
		}

		//Rect{ Cursor::Pos(), 40 }.draw(Palette::Orange);
		
		
	}
}
