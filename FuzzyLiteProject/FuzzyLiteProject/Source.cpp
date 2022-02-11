#include <iostream>
#include <fl/Headers.h>

using namespace fl;

int main()
{
	Engine* engine = new Engine();
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
	RuleBlock* mamdani = new RuleBlock;
	mamdani->setName("mamdani");
	mamdani->setConjunction(fl::null);
	mamdani->setDisjunction(fl::null);
	mamdani->setImplication(new AlgebraicProduct);
	mamdani->setActivation(new General);
	mamdani->addRule(Rule::parse("if obstacle is left then mSteer is right", engine));
	mamdani->addRule(Rule::parse("if obstacle is right then mSteer is left", engine));
	engine->addRuleBlock(mamdani);
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