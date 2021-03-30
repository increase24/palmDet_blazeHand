#include <iostream>
#include <onnxruntime_cxx_api.h>
#include <fstream>
#include "anchors.h"
using namespace std;

int main()
{
    std::cout << "Hello World!\n";
	std::vector<anchorType> anchors;
	generate_anchors(anchors);
	std::cout << anchors.size() << std::endl;
	string palmDetModel = "./models/palmDetection_20210323.onnx";
	Ort::Env* env = new Ort::Env(ORT_LOGGING_LEVEL_WARNING, "test");
	Ort::SessionOptions session_options;
	session_options.SetIntraOpNumThreads(1);
	session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
	std::wstring widstr = std::wstring(palmDetModel.begin(), palmDetModel.end());
	const wchar_t *ModelPath = widstr.c_str();
	Ort::Session* session = new Ort::Session(*env, ModelPath, session_options);

	getchar();
}

