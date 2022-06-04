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
#define VehicleCap 100 //车辆最大载重量
#define  sCusN 7 //模型实际大小
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
		Demand[i] = r.get(10, 20);
	//	cout << "Demand[" << i << "]" << "  =  " << Demand[i]<<endl;
	}
}

void  constridisplay(GRBModel mol,string s1)
{
	GRBConstr st1;
	st1 = mol.getConstrByName(s1);
	cout << st1.get(GRB_StringAttr_ConstrName) << ": "
		<< mol.getRow(st1) << st1.get(GRB_CharAttr_Sense) << st1.get(GRB_DoubleAttr_RHS) << endl;
	cout << endl;
}

int main()
{   dismatrix();
	randdemand();


 try {
	GRBEnv env = GRBEnv(true);
	env.start();
	GRBModel mol = GRBModel(env);
	 int i = sCusN, j = sCusN;
	//vector<vector<vector<double>>> a(isize, vector<double>(jsize, vector<double>(ksize, 0)))
	vector < vector<vector<GRBVar>>> x(i, vector<vector<GRBVar> >(j, vector<GRBVar>(VehicleNum)));
	vector<vector<GRBVar>> y(i, vector<GRBVar>(VehicleNum));
	vector<vector<GRBVar>> u(i, vector<GRBVar>(VehicleNum));
	for (int i = 0; i < sCusN; i++)
	{
		for ( j = 0; j < sCusN; j++)
		{  
			if(i!=j)
			{ 
			   for (int k = 0; k < VehicleNum; k++)
			    {
				x[i][j][k] = mol.addVar(0.0, 1.0, DistanceMatrix[i][j], GRB_BINARY, "x"+to_string(i)+"," + to_string(j)+"," + to_string(k));
			    }
			}
		}
	}

	for (int i = 0; i < sCusN; i++)
	{
		for (int k = 0; k < VehicleNum; k++)
		{
		  y[i][k] = mol.addVar(0.0, 1.0, 0, GRB_BINARY, "y" + to_string(i) + to_string(k));
		}
	}

	for (int i = 1; i < sCusN; i++)
	{
		for (int k = 0; k < VehicleNum; k++)
		{
			u[i][k] = mol.addVar(Demand[i], VehicleCap, 0,  GRB_CONTINUOUS, "u" + to_string(i) + to_string(k));
		}
	}



	GRBLinExpr obj = 0.0;

	for (int i = 0; i < sCusN; i++)
	{
		for (j = 0; j < sCusN; j++)
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

	/*添加约束 ************************************************************/

	GRBLinExpr c1 = 0.0;
	int constrNum = 1;
	for (int i = 0; i < sCusN; i++)
	{
		for (int k = 0; k < VehicleNum; k++)
		{
			c1 += y[i][k];
		}
		mol.addConstr(c1 >=1 , "c1" + to_string(constrNum));
		c1 = 0.0;
		constrNum++;
	}
     cout << "保证每个客户都被服务约束个数" << constrNum - 1 << endl;
	 GRBLinExpr c11 = 0.0;
	 constrNum = 1;
	 for (int i = 0; i < sCusN; i++)
	 {
		 for (int k = 0; k < VehicleNum; k++)
		 {
			 c11 += y[i][k];
		 }
		 mol.addConstr(c11 <= VehicleNum, "c11" + to_string(constrNum));
		 c11 = 0.0;
		 constrNum++;
	 }
	 cout << "保证每个客户都被服务约束个数" << constrNum - 1 << endl;


	 GRBLinExpr c2 = 0.0;
	 constrNum = 1;
	 for (int j = 0; j < sCusN; j++)
	 {
		 for (int k = 0; k < VehicleNum; k++)
		 {
			 for (int i = 0; i < sCusN; i++)
			 {
				 if(i!=j)
				 c2 += x[i][j][k];
			 }
			 mol.addConstr(c2 == y[j][k], "c2" + to_string(constrNum));
			 c2 = 0.0;
			 constrNum++;
		 }
	 }
	 cout << "第1条保证客户被一辆车访问约束个数" << constrNum - 1 << endl;

	 GRBLinExpr c3 = 0.0;
	 constrNum = 1;
	 for (int i = 0; i < sCusN; i++)
	 {
		 for (int k = 0; k < VehicleNum; k++)
		 {
			 for (int j = 0; j < sCusN; j++)
			 {
				 if (i != j)
					 c3 += x[i][j][k];
			 }
			 mol.addConstr(c3 == y[i][k], "c3" + to_string(constrNum));
			 c3 = 0.0;
			 constrNum++;
		 }
	 }
	 cout << "第2条保证客户被一辆车访问约束个数" << constrNum - 1 << endl;

	 GRBLinExpr c4 = 0.0;
	 constrNum = 1;
	 for (int k = 0; k < VehicleNum; k++)
	 {
		 for (int i = 0; i < sCusN; i++)
		 {

			 c4 += Demand[i] * y[i][k];

		 }
		 mol.addConstr(c4 <= VehicleCap, "c4" + to_string(constrNum));
		 c4 = 0.0;
		 constrNum++;
	 }
	 cout << "不超载重约束约束个数" << constrNum - 1 << endl;

    /*
	 GRBLinExpr c9 = 0.0;
	 constrNum = 1;
	 for (int k = 0; k < VehicleNum; k++)
	 {
	   c9 +=  y[0][k]; 
	 }
	 mol.addConstr(c9 <= VehicleNum, "c9" + to_string(constrNum));
	 cout << "所有车必须派出约束个数" << constrNum  << endl;
     */
/*
	 GRBLinExpr c5 = 0.0;
	 constrNum = 1;
         for (j = 1; j < sCusN; j++)
		 {
			 
			for (int k = 0; k < VehicleNum; k++)
				 {
					 c5 += x[0][j][k];
				 }
			 mol.addConstr(c5 == 1, "c5" + to_string(constrNum));
		     constrNum++;
		     c5 = 0;
		 }
		
	 
	 cout << "约束个数" << constrNum - 1 << endl;

	 GRBLinExpr c6 = 0.0;
	 constrNum = 1;
	 for (int i = 1; i < sCusN; i++)
	 {
		 for (int k = 0; k < VehicleNum; k++)
		 {
		   c6 += x[i][0][k];
		   }
			
		 mol.addConstr(c6 == 1, "c6" + to_string(constrNum));
		 constrNum++;
		 c6 = 0;
	 }
	 cout << "约束个数" << constrNum - 1 << endl;


	 GRBLinExpr c7 = 0.0,c8=0;
	 constrNum = 1;
	 for (int i = 0; i < sCusN; i++)
	 {
		 for (j = 0; j < sCusN; j++)
		 {
			 if (i != j)
			 {
				 for (int k = 0; k < VehicleNum; k++)
				 {
					 c7+=x[i][j][k];
					 c8+=x[j][i][k];
				 }
			 }
		 }
		 mol.addConstr(c7 == c8, "c78" + to_string(constrNum));
		 constrNum++;
		 c7 = 0;
		 c8 = 0;


	 }
	 cout << "约束个数" << constrNum - 1 << endl;
*/




	 constrNum = 1;
	 for (int i = 1; i < sCusN; i++)
	 {
		 for (j = 1; j < sCusN; j++)
		 {
			 if (i != j)
			 {
				 for (int k = 0; k < VehicleNum; k++)
				 {
					 mol.addConstr(u[i][k] - u[j][k] + VehicleCap * x[i][j][k] <= VehicleCap - Demand[j], "c6" + to_string(constrNum));
					 constrNum++;
				 }
			 }
		 }
	 }
	 cout << "子回路消除约束个数" << constrNum - 1 << endl;


      string  iis="iis";
	 //mol.computeIIS();

	mol.optimize();	
   // mol.write("不可行iis.ilp");
	//cout << mol.getObjective() << endl;//获取目标函数


	for (int i = 0; i < sCusN; i++)
	{
		for (j = 0; j < sCusN; j++)
		{
			if (i != j)
			{
				for (int k = 0; k < VehicleNum; k++)
				{
					if(x[i][j][k].get(GRB_DoubleAttr_X) ==1)
					cout << x[i][j][k].get(GRB_StringAttr_VarName) << " =  "
						<< x[i][j][k].get(GRB_DoubleAttr_X) << "     " << y[i][k].get(GRB_StringAttr_VarName) << " = "
						<< y[i][k].get(GRB_DoubleAttr_X)<<endl;
				    
				}
			}
		}
	}

	
	constridisplay(mol,  "c21");
	//constridisplay(mol,  "c12");
	











	 }
	 catch (GRBException e) {
		 cout << "Error code = " << e.getErrorCode() << endl;
		 cout << e.getMessage() << endl;
	 }
	 catch (...) {
		 cout << "Error during optimization" << endl;
	 }





	return 0;
}