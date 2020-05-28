#include <vector>
#include <fstream>

#include "pch.h"
#include "CppUnitTest.h"

#include "../GeneticAlgorithm/Reader.h"
#include "../GeneticAlgorithm/Writer.h"

#define INFILE "intest.txt"
#define OUTFILE "outtest.txt"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TravellingSalesmanTest
{
	TEST_CLASS(IOTest) {
	public:

		TEST_METHOD(READ_TEST)
		{
			Reader r(INFILE);
			std::vector<std::pair<float, float>> data = r.parse();

			Assert::AreEqual(20833.3333f, data[0].first);
			Assert::AreEqual(13433.3333f, data[17].second);
			Assert::AreEqual(21600.0000f, data[3].first);
			Assert::AreEqual(26133.3333f, data[14].first);
			Assert::AreEqual(12716.6667f, data[8].second);
			Assert::AreEqual(12992.2222f, data[28].second);
		}

	private:
		std::fstream infile, outfile;
	};
}