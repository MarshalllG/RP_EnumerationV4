COMP_OPT = -Wall -Wextra -Wfatal-errors -std=c++11 -O3

RP_Test.exe: RP_Driver.o RP_Enumeration.o Router_placement_Data.o Random.o
	g++ -o RP_Test.exe RP_Driver.o RP_Enumeration.o Router_placement_Data.o Random.o

RP_Enumeration.o: RP_Enumeration.cc RP_Enumeration.hh Router_placement_Data.hh
	g++ $(OPTIONS) -c RP_Enumeration.cc

Router_placement_data.o: Router_placement_Data.cc Router_placement_Data.hh
	g++ $(COMP_OPT) -c Router_placement_Data.cc 

Random.o: Random.cc Random.hh
	g++ $(OPTIONS) -c Random.cc

RP_Driver.o: RP_Driver.cc Router_placement_Data.hh RP_Enumeration.hh EnumerationOpt.hh
	g++ $(COMP_OPT) -c RP_Driver.cc

clean:
	rm -f Router_placement_data.o RP_Enumeration.o RP_Driver.o Random.o RP_Test.exe

