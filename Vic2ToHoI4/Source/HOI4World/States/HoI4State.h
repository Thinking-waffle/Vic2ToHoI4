/*Copyright (c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/



#ifndef HOI4STATE_H_
#define HOI4STATE_H_



#include "newParser.h"
#include "../../Configuration.h"
#include "../../Mappers/CountryMapping.h"
#include "../../Mappers/Provinces/ProvinceMapper.h"
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>


namespace Vic2
{
class Province;
class State;
}


namespace HoI4
{

class coastalProvinces;
class StateCategories;


class State
{
	public:
		State(const Vic2::State* sourceState, int _ID, const std::string& _ownerTag);

		void output(std::ostream& output, const Configuration& theConfiguration) const;

		void addProvince(int province) { provinces.insert(province); }
		void setAsCapitalState() { capitalState = true; civFactories++; }
		void makeImpassable() { impassable = true; }
		void markHadImpassablePart() { hadImpassablePart = true; }
		void addResource(const std::string& resource, double amount)	{ resources[resource] += amount; }
		void addAirBase(int newAirBase) { airbaseLevel += newAirBase; if (airbaseLevel > 10) airbaseLevel = 10; }
		void addVictoryPointValue(int additionalValue) { victoryPointValue += additionalValue; }
		void setVPLocation(int province) { victoryPointPosition = province; }

		void convertNavalBases(const coastalProvinces& theCoastalProvinces, const provinceMapper& theProvinceMapper);
		void addNavalBase(int level, int location);
		void addCores(const std::set<std::string>& newCores);
		void convertControlledProvinces(const provinceMapper& theProvinceMapper, const CountryMapper& countryMapper);

		const Vic2::State* getSourceState() const { return sourceState; }
		int getID() const { return ID; }
		std::set<int>getProvinces() const { return provinces; }
		std::string getOwner() const { return ownerTag; }
		std::set<std::string> getCores() const { return cores; }
		bool isImpassable() const { return impassable; }
		int getDockyards() const { return dockyards; }
		int getCivFactories() const { return civFactories; }
		int getMilFactories() const { return milFactories; }
		int getInfrastructure() const { return infrastructure; }
		int getManpower() const { return manpower; }
		std::optional<int> getVPLocation() const { return victoryPointPosition; }
		std::set<int> getDebugVPs() const { return debugVictoryPoints; }
		std::set<int> getSecondaryDebugVPs() const { return secondaryDebugVictoryPoints; }

		std::optional<int> getMainNavalLocation() const;

		void tryToCreateVP(const provinceMapper& theProvinceMapper, const Configuration& theConfiguration);
		void addManpower(const provinceMapper& theProvinceMapper, const Configuration& theConfiguration);

		void convertIndustry(
			double workerFactoryRatio,
			const HoI4::StateCategories& theStateCategories,
			const coastalProvinces& theCoastalProvinces
		);

	private:
		int determineFactoryNumbers(double workerFactoryRatio) const;
		int constrainFactoryNumbers(double rawFactories) const;
		void determineCategory(int factories, const HoI4::StateCategories& theStateCategories);
		void setInfrastructure(int factories);
		void setIndustry(int factories, const coastalProvinces& theCoastalProvinces);
		bool amICoastal(const coastalProvinces& theCoastalProvinces) const;

		int determineNavalBaseLevel(const Vic2::Province& sourceProvince) const;
		std::optional<int> determineNavalBaseLocation(
			const Vic2::Province& sourceProvince,
			const coastalProvinces& theCoastalProvinces,
			const provinceMapper& theProvinceMapper
		) const;

		bool assignVPFromVic2Province(int Vic2ProvinceNumber, const provinceMapper& theProvinceMapper);
		void assignVP(int location);
		bool isProvinceInState(int provinceNum) const;
		void addDebugVPs(const provinceMapper& theProvinceMapper);

		const Vic2::State* sourceState;

		int ID = 0;
		std::set<int> provinces;
		std::string ownerTag;
		std::set<std::string> cores;
		std::map<std::string, std::set<int>> controlledProvinces;

		bool capitalState = false;
		bool impassable = false;
		bool hadImpassablePart = false;

		int manpower = 0;

		int civFactories = 0;
		int milFactories = 0;
		int dockyards = 0;
		std::string category = "pastoral";
		int infrastructure = 0;

		std::map<int, int> navalBases;

		int airbaseLevel = 0;

		std::map<std::string, double> resources;

		std::optional<int> victoryPointPosition;
		int victoryPointValue = 0;
		std::set<int> debugVictoryPoints;
		std::set<int> secondaryDebugVictoryPoints;

		std::map<int, int> landFortLevels;
		std::map<int, int> coastFortLevels;
};

}



#endif // HOI4STATE_H_
