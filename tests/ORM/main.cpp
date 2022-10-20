#include <moka/moka.h>
#include <fakeit.hpp>

int main() {
	Moka::Report report;
	Moka::Context("WebFrame", [](Moka::Context& it) {
		it.describe("ORM", [](Moka::Context& it) {
			it.should("run tests", []() {
				must_be_greater(0, 1, "No tests available");
			});
			it.should("run tests", []() {
				must_be_greater(0, -1, "No tests available");
			});
		});
	}).test(report);

	return report.print();
}