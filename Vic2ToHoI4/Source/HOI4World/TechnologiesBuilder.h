#ifndef TECHNOLOGIES_BUILDER_H
#define TECHNOLOGIES_BUILDER_H



#include "Technologies.h"
#include <memory>



namespace HoI4
{

class technologies::Builder
{
  public:
	Builder() { theTechnologies = std::make_unique<technologies>(); }
	std::unique_ptr<technologies> Build() { return std::move(theTechnologies); }

	Builder& addTechnology(const std::string& technology)
	{
		theTechnologies->mainTechnologies.insert(technology);
		return *this;
	}

  private:
	std::unique_ptr<technologies> theTechnologies;
};

} // namespace HoI4



#endif // TECHNOLOGIES_BUILDER_H