#include<iostream>
#include<vector>
#include"gurobi_c++.h"
#include"LP1.h"
#include <ctime>
#include <cstdlib>
#include<string>
using namespace std;

#define VehicleNum 3 //车辆数
#define CustomNum  30//客户数量
#define VehicleCap 200 //车辆最大载重量
vector<vector<double>>DistanceMatrix(CustomNum, vector<double>(CustomNum));//距离矩阵用vector 指定长度
vector<int>Demand(CustomNum); //需求矩阵
const double CustomCoordinate[][2] = {//客户点坐标
{23,95},  {820,915},{470,302},{950,905},{868,744},
{891,987},{76,625},  {8,541},{219,988},{662,636},
{318,402},{307,984},{689,486},{675,946},{665,331},
{392,143},{957,725},{442,911},{576,762},{985,369},
{170,460},{861,246},{192,61},{121,1000},{858,541},
{261,204},{572,74},{788,22},{303,694},{97,241}
};

class RandomNumber {
public:
	RandomNumber() {
		srand(time(0));
	}
	int get(int begin = 0, int end = 1) {
		return rand() % (end - begin + 1) + begin;
	}
};



double  discal(const double* city1, const double* city2)
{
	double x1 = city1[0];			//城市1横坐标
	double y1 = city1[1];
	double x2 = city2[0];
	double y2 = city2[1];			//城市2纵坐标
	double dist = pow((pow((x1 - x2), 2) + pow((y1 - y2), 2)), 0.5);
	return dist;

}

void dismatrix()
{
	for (int i = 0; i < CustomNum; i++)
	{
		for (int j = 0; j < CustomNum; j++)
		{
			if (i == j)
				DistanceMatrix[i][j]=0.0;//相同客户点的为0
			DistanceMatrix[i][j] = DistanceMatrix[j][i] = discal(CustomCoordinate[i], CustomCoordinate[j]);
		}
	}
  
}

void  randdemand()
{
	Demand[0] = 0;
	RandomNumber r;
	for (int i = 1; i < CustomNum; i++)
	{
		Demand[i] = r.get(10, 99);
	//	cout << "Demand[" << i << "]" << "  =  " << Demand[i]<<endl;
	}
}

int main()
{   dismatrix();
	randdemand();
	GRBEnv env = GRBEnv(true);
	env.start();
	GRBModel mol = GRBModel(env);
	 int i = 11, j = 11;
	//vector<vector<vector<double>>> a(isize, vector<double>(jsize, vector<double>(ksize, 0)))
	vector < vector<vector<GRBVar>>> x(i, vector<vector<GRBVar> >(j, vector<GRBVar>(VehicleNum)));
	vector<vector<GRBVar>> y(i, vector<GRBVar>(VehicleNum));
	for (int i = 0; i < 11; i++)
	{
		for ( j = 0; j <11; j++)
		{  
			if(i!=j)
			{ 
			   for (int k = 0; k < VehicleNum; k++)
			    {
				x[i][j][k] = mol.addVar(0.0, 1.0, DistanceMatrix[i][j], GRB_BINARY, "x"+to_string(i) + to_string(j) + to_string(k));
			    }
			}
		}
	}

	for (int i = 0; i < 11; i++)
	{
		for (int k = 0; k < VehicleNum; k++)
		{
		  y[i][k] = mol.addVar(0.0, 1.0, 0, GRB_BINARY, "y"+to_string(i)+to_string(k));
		}
	}

	GRBLinExpr obj = 0.0;

	for (int i = 0; i < 11; i++)
	{
		for (j = 0; j < 11; j++)
		{
			if (i != j)
			{
				for (int k = 0; k < VehicleNum; k++)
				{
					obj += DistanceMatrix[i][j] * x[i][j][k];
				}
			}
		}
	}
	mol.setObjective(obj, GRB_MINIMIZE);





	GRBLinExpr c1 = 0.0;
	for (int i = 1; i < 11; i++)
	{
		for (int k = 0; k < VehicleNum; k++)
		{
			c1 += y[i][k];
		}
		mol.addConstr(c1 == 1, "c1"+to_string(i));
		c1 = 0.0;
	}

	GRBLinExpr c2 = 0.0;
	for (int k = 0; k < VehicleNum; k++)
	{
		c2 += y[0][k];
	}
	mol.addConstr(c2 == VehicleNum, "c2");
	GRBLinExpr c3 = 0.0;
	for (int k = 0; k < VehicleNum; k++)
	{
		for (int i = 1; i < 11; i++)
		{
			c3 += Demand[i] * y[i][k];
		}
		mol.addConstr(c3 <= VehicleCap, "c3" + to_string(k));
		c3 = 0.0;
	}

	for (int i = 0; i < 11; i++)
	{
		for (j = 0; j < 11; j++)
		{
			if (i != j)
			{
				for (int k = 0; k < VehicleNum; k++)
				{
					mol.addConstr( x[i][j][k] ==y[i][k], "c4" + to_string(i));
				}
			}
		}
	}

	for (int j = 0; j < 11; j++)
	{
		for (i = 0; i < 11; i++)
		{
			if (i != j)
			{
				for (int k = 0; k < VehicleNum; k++)
				{
					mol.addConstr(x[j][i][k] == y[j][k], "c5" + to_string(j));
				}
			}
		}
	}










	mol.optimize();	
	GRBConstr st1;
	//cout << mol.getObjective() << endl;//获取目标函数

	

	st1= mol.getConstrByName("c2");
	cout << st1.get(GRB_StringAttr_ConstrName)<<": "
	  <<mol.getRow(st1)<<st1.get(GRB_CharAttr_Sense)<<st1.get(GRB_DoubleAttr_RHS)<<endl;

	st1 = mol.getConstrByName("c11");
	cout << st1.get(GRB_StringAttr_ConstrName) << ": "
		<< mol.getRow(st1) << st1.get(GRB_CharAttr_Sense) << st1.get(GRB_DoubleAttr_RHS)<<endl;

	st1 = mol.getConstrByName("c12");
	cout << st1.get(GRB_StringAttr_ConstrName) << ": "
		<< mol.getRow(st1) << st1.get(GRB_CharAttr_Sense) << st1.get(GRB_DoubleAttr_RHS) << endl;

	st1 = mol.getConstrByName("c30");
	cout << st1.get(GRB_StringAttr_ConstrName) << ": "
		<< mol.getRow(st1) << st1.get(GRB_CharAttr_Sense) << st1.get(GRB_DoubleAttr_RHS) << endl;


	st1 = mol.getConstrByName("c31");
	cout << st1.get(GRB_StringAttr_ConstrName) << ": "
		<< mol.getRow(st1) << st1.get(GRB_CharAttr_Sense) << st1.get(GRB_DoubleAttr_RHS) << endl;

	st1 = mol.getConstrByName("c32");
	cout << st1.get(GRB_StringAttr_ConstrName) << ": "
		<< mol.getRow(st1) << st1.get(GRB_CharAttr_Sense) << st1.get(GRB_DoubleAttr_RHS) << endl;

	st1 = mol.getConstrByName("c41");
	cout << st1.get(GRB_StringAttr_ConstrName) << ": "
		<< mol.getRow(st1) << st1.get(GRB_CharAttr_Sense) << st1.get(GRB_DoubleAttr_RHS) << endl;
	return 0;
}