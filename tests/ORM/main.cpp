#include "../lib/moka/moka.h"
#include "../lib/fakeit.hpp"

int main() {
	Moka::Report report;
	Moka::Context("WebFrame", [](Moka::Context& it) {
		it.describe("ORM", [](Moka::Context& it) {
			it.should("run tests", []() {
			});
		});
	}).test(report);

	return report.print();
}