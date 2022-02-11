#include <iostream>
#include <fl/Headers.h>

using namespace fl;

int main()
{
	Engine* engine = new Engine();
	Engine* engine = FisImporter().fromFile("Filename.fis");
	///////////////////////
	InputVariable* obstacle = new InputVariable;
	engine->addInputVariable(obstacle);
	///////////////////////
	obstacle->setName("obstacle");
	obstacle->setRange(0.000, 1.000);
	///////////////////////
	obstacle->addTerm(new Ramp("left", 1.000, 0.000));
	obstacle->addTerm(new Ramp("right", 0.000, 1.000));
	///////////////////////
	OutputVariable* mSteer = new OutputVariable;
	engine->addOutputVariable(mSteer);
	mSteer->setName("mSteer");
	mSteer->setRange(0.000, 1.000);
	mSteer->setAggregation(new Maximum);
	mSteer->setDefuzzifier(new Centroid(100));
	mSteer->setDefaultValue(fl::nan);
	mSteer->addTerm(new Ramp("left", 1.000, 0.000));
	mSteer->addTerm(new Ramp("right", 0.000, 1.000));
	///////////////////////


	///////////////////////
	InputVariable* speed = new InputVariable;
	engine->addInputVariable(speed);
	speed->setName("speed");
	speed->setRange(0.000, 1.000);
	speed->addTerm(new Ramp("movig_left", 1.000, 0.600));
	speed->addTerm(new Ramp("moving_right", 0.300, 1.000));
	speed->addTerm(new Triangle("none", 0.250, 0.750));
	///////////////////////
	OutputVariable* mSpeedSteer = new OutputVariable;
	engine->addOutputVariable(mSpeedSteer);
	mSpeedSteer->setName("mSteer");
	mSpeedSteer->setRange(0.000, 1.000);
	mSpeedSteer->setAggregation(new Maximum);
	mSpeedSteer->setDefuzzifier(new Centroid(100));
	mSpeedSteer->setDefaultValue(fl::nan);
	mSpeedSteer->addTerm(new Ramp("left", 1.000, 0.600));
	mSpeedSteer->addTerm(new Ramp("right", 0.300, 1.000));
	mSpeedSteer->addTerm(new Triangle("none", 0.250, 0.750));
	///////////////////////
	RuleBlock* mamdani = new RuleBlock;
	mamdani->setName("mamdani");
	mamdani->setConjunction(new AlgebraicProduct);
	mamdani->setDisjunction(new Maximum);
	mamdani->setImplication(new AlgebraicProduct);
	mamdani->setActivation(new General);
	mamdani->addRule(Rule::parse("if obstacle is left then mSteer is right", engine));
	mamdani->addRule(Rule::parse("if obstacle is right then mSteer is left", engine));
	mamdani->addRule(Rule::parse("if obstacle is left or speed is left then mSteer is right", engine));
	mamdani->addRule(Rule::parse("if obstacle is right or speed is right then mSteer is left", engine));
	engine->addRuleBlock(mamdani);
	///////////////////////
	// 
	///////////////////////
	while (1)
	{
		std::cout << "0-1 Range: Left = 0, Right = 1 " << std::endl;
		std::string input = "";
		std::cin >> input;
		std::stringstream ss(input);
		float number = 0.f;
		ss >> number;

		obstacle->setValue(number);
		engine->process();
		std::cout << "obstacle.input = " << number <<
			" => steer.output = " << mSteer->getValue() << std::endl << std::endl;
	}
	//////////////////////

	std::string status;
	if (not engine->isReady(&status))
		std::cout << "Engine is not ready" << std::endl << status << std::endl;

	delete engine;
	return 0;
}
//https://www.fuzzylite.com/cpp/

/*
highlight  shift alt drag - change multiple lines at once

*/