// // RP_Greedy.hh
// #ifndef _RP_GREEDY_HH_
// #define _RP_GREEDY_HH_
// #include "Router_placement_data.hh"

// bool IsFeasibleRouter (const RP_Input& in, RP_Output& out, point r);
// point MakeRandomMove(point p);
// point SelectRandomRouter(const RP_Input& in, RP_Output& out);
// unsigned PlaceRouter(const RP_Input& in, RP_Output& out);
// void ConnectRouterWithBackbone(RP_Output& out, const RP_Input& in, point router);
// void GreedyRP(const RP_Input& in, RP_Output& out);

// bool PointIsInVec (point pt, vector<point> vec);
// void PrintVec (vector<point> vec);
// #endif


#include "EnumerationOpt.hh"
#include "Router_placement_data.hh"

// insert here the definition of the class EnumerationRPOpt
class EnumerationRPOpt : public EnumerationOpt<RP_Input, RP_Output, int>
{
public:
  EnumerationRPOpt(const RP_Input& in) : EnumerationOpt(in) {}
protected:
  void First() override;
  bool Next() override;
  bool Feasible() override;
  int Cost() override;
};

