//  Router_placement_data.cpp

#include "Router_placement_data.hh"

unsigned ManhattanDistance(point p1, point p2)
{
    return (abs(int(p1.row - p2.row)) + abs(int(p1.col - p2.col)));
}

unsigned InfDistance (point p1, point p2)
{
    return max(abs(p1.row - p2.row), abs(p1.col - p2.col));
}

bool ThereAreWallsInside (const RP_Input& in, point p1, point p2)
{
    for (unsigned r=min(p1.row, p2.row); r <= max (p1.row, p2.row); r++)
    {
        for (unsigned c=min(p1.col, p2.col); c <= max (p1.col, p2.col); c++)
        {
            if (in.IsWall(r,c))
                return 1;
        }
    }
    return 0;
}


RP_Input::RP_Input(string file_name)
{  
    char ch;

    ifstream is(file_name);
    if (!is) 
    {
        cerr << "Cannot open file " <<  file_name << endl;
	    exit(1);
    }

    is >> rows >> columns >> radius
       >> b_price >> r_price >> budget
       >> start_backbone.row >> start_backbone.col;

    cells.resize(rows, vector<char>(columns));

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < columns; c++)
        {
            is >> ch;
            cells[r][c] = ch;
        }
    }
}


ostream& operator<<(ostream& os, const RP_Input& in)
{
    for (int r = 0; r < in.rows; r++)
    {
        for (int c = 0; c < in.columns; c++)
        {
            os << in.cells[r][c];
        }
        cout << endl;
    }
    return os;
}


RP_Output::RP_Output(const RP_Input& my_in) : in(my_in) 
{
    out_cells = in.Cells(); // initialize grid

    backbone.push_back(in.StartBackbone()); // initialize backbone
    out_cells[in.StartBackbone().row][in.StartBackbone().col] = 'b';
    remaining_budget = in.Budget()-in.BackbonePrice();

    total_covered_points = 0;
}


void RP_Output::InsertRouter(unsigned r, unsigned c)
{
    point router;
    vector<point> router_coverage;

    // if (!in.IsWall(r, c))
    // {
        // update cell
        out_cells[r][c] = 'r';

        router.row = r;
        router.col = c;
    
        routers.push_back(router); // update routers vector
        // cout << "Placed router in cell " << router.row << " " << router.col << endl;

        // // update coverage
        // router_coverage = FindCoverageArea(router); 
        // covered.push_back(router_coverage);

        // // cout << "Il router copre " << router_coverage.size() << " celle." << endl;
        // total_covered_points += router_coverage.size();
        // for (unsigned i = 0; i < router_coverage.size(); i++)
        //     if (out_cells[router_coverage[i].row][router_coverage[i].col] != 'r')
        //         out_cells[router_coverage[i].row][router_coverage[i].col] = '1';
            
        // update budget
        remaining_budget -= in.RouterPrice();
        UpdateBackbone();
}


void RP_Output::InsertBackboneCell(unsigned r, unsigned c)
{
    point bb_cell;
    bb_cell.row = r;
    bb_cell.col = c;

    if (!IsInBackbone(bb_cell))
        backbone.push_back(bb_cell);

    // update budget
    remaining_budget -= in.BackbonePrice();
}


// bool RP_Output::IsCovered(point pt) const
// {
//     vector<point> router_coverage;

//     if (RoutersSize() == 0)
//         return 0;

//     for (unsigned i = 0; i < RoutersSize(); i++)
//     {
//         router_coverage = FindCoverageArea(routers[i]);
//         for (unsigned j = 0; j < router_coverage.size(); j++)
//         {
//             if (pt.row == router_coverage[j].row && pt.col == router_coverage[j].col)
//             { 
//                 return 1;
//             }
//         }
//     }
//     return 0;
// }


// dato un punto, restituisce che celle coprirebbe se fosse un router.
// vector<point> RP_Output::FindCoverageArea(point router) const
// {
//     point temp;
//     unsigned step = 1;
//     vector<point> coverage;

//     while (step <= in.Radius())
//     {
//         for (int r=router.row-step; r <= router.row+step; r++)
//         {
//             for (int c=router.col-step; c <= router.col+step; c++)
//             {
//                 if (in.IsGridPoint(r,c))
//                 {
//                     temp.row = r;
//                     temp.col = c;
//                     if (in.IsDot(r,c) && !ThereAreWallsInside(in, temp, router))
//                     {
//                         if (InfDistance(temp, router) == step || (r == router.row && c == router.col))
//                         {
//                             for (unsigned j = 0; j < coverage.size(); j++)
//                                 if (temp.row == coverage[j].row && temp.col == coverage[j].col)
//                                     break;
                            
//                                 coverage.push_back(temp);

//                             // cout << "Aggiungo ai covered temp: " << temp.row << " " << temp.col << endl;
//                         }
//                     }
//                 }
//             }
//         }
//         step++;
//     } 
//     return coverage;
// }

// dato un punto, restituisce che celle coprirebbe se fosse un router.
vector<point> RP_Output::FindCoverageArea(point router) const
{
    point temp;
    unsigned step = 0;
    vector<point> coverage;

    if (in.IsDot(router.row, router.col))
        coverage.push_back(router);

    while (step <= in.Radius())
    {
        for (int r=router.row-step; r <= router.row+step; r++)
        {
            for (int c=router.col-step; c <= router.col+step; c++)
            {
                if (in.IsGridPoint(r,c))
                {
                    temp.row = r;
                    temp.col = c;
                    
                    if (out_cells[r][c] == '.' && !ThereAreWallsInside(in, temp, router))
                    {
                        if (InfDistance(temp, router) == step)
                            coverage.push_back(temp);
                    }
                }
            }
        }
        step++;
    } 
    return coverage;
}


ostream& operator<<(ostream& os, const RP_Output& out)
{
    for (unsigned r = 0; r < out.Rows(); r++)
    {
        for (unsigned c = 0; c < out.Columns(); c++)
        {
            os << out.out_cells[r][c];
        }
        cout << endl;
    }
    return os;
}


bool RP_Output::IsInBackbone (point pt) const
{
    for (unsigned i = 0; i < backbone.size(); i++)
        if (pt.row == backbone[i].row && pt.col == backbone[i].col)
            return 1;

    return 0;
}


point RP_Output::ClosestBackbonePoint (point router) const
{
    unsigned minimum_distance = ManhattanDistance(in.StartBackbone(), router);
    point closest = in.StartBackbone();
    unsigned current_distance;

    for (unsigned i = 0; i < BackboneSize(); i++)
    {
        current_distance = ManhattanDistance(BackboneCell(i), router);
        if (current_distance < minimum_distance)
        {
            closest = BackboneCell(i);
            minimum_distance = current_distance;
        }
    }
    
    return closest;
}


// move from p to one point of his neighborhood
point RP_Output::MakeRandomMove(point p) 
{  
    point p2;

    p2.row = p.row + Random(-1, 1);
    p2.col = p.col + Random (-1, 1);

    return p2;
}


void RP_Output::ConnectRouterWithBackbone(point router)
{
    point temp, selected_bb_cell;
    unsigned new_distance;

    // select the backbone point
    selected_bb_cell = ClosestBackbonePoint(router);
    // cout << "selectedbbcell: " << selected_bb_cell.row << " " << selected_bb_cell.col << endl;

    unsigned current_distance = ManhattanDistance(selected_bb_cell, router);

    while (current_distance > 0)
    {
        temp = MakeRandomMove(selected_bb_cell);
        new_distance = ManhattanDistance(temp, router);
        if (new_distance < current_distance)
        {
            current_distance = new_distance;
            InsertBackboneCell(temp.row, temp.col);
            selected_bb_cell = temp;
        }
    }
}

void RP_Output::RemoveRouter(point router)
{
    vector<point> to_uncover;
    unsigned i;

    // search router in routers
    for (i = 0; i < routers.size(); i++)
    {
        if (routers[i].row == router.row && routers[i].col == router.col)
        {
            routers.erase(routers.begin()+i);
            break;
        }
    }

    // cout << "router: " << router.row << " " << router.col << endl;
    // for (unsigned k = 0; k < routers.size(); k++)
    // {
    //     cout << "routers[" << k << "]: " << routers[k].row << " " << routers[k].col << endl;
    // }

    // update cells
    out_cells[router.row][router.col] = in.Cells()[router.row][router.col];

    // // update coverage
    // to_uncover = FindCoverageArea(router); 
    // total_covered_points -= to_uncover.size();
    // for (unsigned i = 0; i < to_uncover.size(); i++)
    //     out_cells[to_uncover[i].row][to_uncover[i].col] = in.Cell(to_uncover[i].row, to_uncover[i].col);

    // update budget
    remaining_budget += in.RouterPrice();
    UpdateBackbone();
}

// void RP_Output::RemoveLastRouter()
// {
//     // update cells
//     unsigned last = covered.size()-1;
//     for (unsigned i = 0; i < covered[last].size(); i++)
//         out_cells[covered[last][i].row][covered[last][i].col] = in.Cell(covered[last][i].row, covered[last][i].col);

//     // update coverage
//     total_covered_points -= covered[last].size();
//     covered.pop_back();

//     // update routers
//     routers.pop_back();
// }


void RP_Output::RemoveAllRoutersAndRestartBackbone()
{
    // remove all routers
    routers.resize(0);

    // restart with initial budget
    ResetBudget();

    // remove all covered
    for (unsigned i = 0; i < covered.size(); i++)
        covered.pop_back();

    // update out_cells
    for (unsigned r = 0; r < in.Rows(); r++)
        for (unsigned c = 0; c < in.Columns(); c++)
            if (out_cells[r][c] == 'r')
            {
                out_cells[r][c] = in.Cell(r, c);
            }
    UpdateBackbone();
}


void RP_Output::UpdateBackbone()
{
    remaining_budget += backbone.size()*in.BackbonePrice();
    // delete backbone
    backbone.resize(0);
    backbone.push_back(in.StartBackbone());
    remaining_budget -= in.BackbonePrice();

    for (unsigned i = 0; i < routers.size(); i++)
        ConnectRouterWithBackbone(routers[i]);
}

void RP_Output::ResetBudget()
{
    remaining_budget = in.Budget();
}

void RP_Output::UpdateCoverage()
{
    vector<point> coverage;
    covered.resize(0);
    total_covered_points = 0;

    for (unsigned r = 0; r < in.Rows(); r++)
        for (unsigned c = 0; c < in.Columns(); c++)
            if (out_cells[r][c] == '1')
                out_cells[r][c] = '.';

    for (unsigned i = 0; i < routers.size(); i++)
    {
        coverage = FindCoverageArea(routers[i]);
        // cout << "FindCov: " << coverage.size() << endl;
        covered.push_back(coverage);
        total_covered_points += coverage.size();

        for (unsigned i = 0; i < coverage.size(); i++)
            if (out_cells[coverage[i].row][coverage[i].col] != 'r')
                out_cells[coverage[i].row][coverage[i].col] = '1';
    }
}

 
// void RP_Output::RemoveBackboneCell (point bb_cell)
// {
//     backbone.delete()
//     out.remaining_budget += in.BackbonePrice();
// }

RP_Output& RP_Output::operator=(const RP_Output& out)
{   
    out_cells = out.out_cells;
    routers = out.routers;
    backbone = out.backbone;
    covered = out.covered;
    total_covered_points = out.total_covered_points;
    remaining_budget = out.remaining_budget;
    return *this;
}


point RP_Output::NextCell(unsigned r, unsigned c) const
{
    // if (c == in.Columns()-1 && r == in.Rows()-1)
    //     exit (EXIT_FAILURE);
    if (c == in.Columns()-1 && r != in.Rows()-1)
    {
        r++;
        c = 0;
    }
    else
        c++;

    point next;
    next.col = c;
    next.row = r;

    return next;
}

point RP_Output::PreviousCell(unsigned r, unsigned c) const
{
    // if (c == 0 && r == 0)
    //     exit (EXIT_FAILURE);
    if (c == 0 && r != 0)
    {
        r--;
        c = in.Columns()-1;
    }
    else
        c--;

    point previous;
    previous.col = c;
    previous.row = r;

    return previous;
}