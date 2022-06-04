#pragma once
/*


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
					mol.addConstr(x[i][j][k] - x[j][i][k]==0, "c4" + to_string(i));
				}
			}
		}
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


GRBLinExpr c1 = 0.0;
int constrNum = 1;
for (int i = 0; i < 11; i++)
{
	for (j = 0; j < 11; j++)
	{
		if (i != j)
		{
			for (int k = 0; k < VehicleNum; k++)
			{
				c1 += x[i][j][k];
			}
		}
	}
	mol.addConstr(c1 == 1, "c1" + to_string(constrNum));
	constrNum++;
	c1 = 0;
}
cout << "约束个数" << constrNum - 1 << endl;

GRBLinExpr c7 = 0.0;
constrNum = 1;
for (int j = 0; j < 11; j++)
{
	for (i = 0; i < 11; i++)
	{
		if (i != j)
		{
			for (int k = 0; k < VehicleNum; k++)
			{
				c7 += x[j][i][k];
			}
		}
	}
	mol.addConstr(c7 == 1, "c7" + to_string(constrNum));
	constrNum++;
	c7 = 0;
}
cout << "约束个数" << constrNum - 1 << endl;












    
GRBLinExpr c2 = 0.0;
constrNum = 1;
for (int k = 0; k < VehicleNum; k++)
{
	for (j = 1; j < 11; j++)
	{
		c2 += x[0][j][k];

	}
	mol.addConstr(c2 == 1, "c2" + to_string(constrNum));
	c2 = 0;
	constrNum++;
}
cout << "约束个数" << constrNum - 1 << endl;





GRBLinExpr c3 = 0.0;
constrNum = 1;
for (int k = 0; k < VehicleNum; k++)
{
	for (i = 1; i < 11; i++)
	{
		c3 += x[i][0][k];

	}
	mol.addConstr(c3 == 1, "c3" + to_string(constrNum));
	c3 = 0;
	constrNum++;
}
cout << "约束个数" << constrNum - 1 << endl;


	GRBLinExpr c41 = 0.0,c42=0.0;
	constrNum = 1;
	for (int k = 0; k < VehicleNum; k++)
	{
		for (i = 0; i <11; i++)
		{
			   for (int j = 0; j <11; j++)
			  {
					if (i != j)
				   {
					   c41 += x[i][j][k];
					   c42 += x[j][i][k];
					}
			   }

			   mol.addConstr(c41 == c42, "c4" +to_string(constrNum));
			   c41 = 0.0;
			   c42 = 0.0;
			   constrNum++;
		}

	}
	cout << "约束个数" << constrNum - 1 << endl;



GRBLinExpr c5 = 0.0;
constrNum = 1;
for (int k = 0; k < VehicleNum; k++)
{
	for (int i = 0; i < 11; i++)
	{

		c5 += Demand[i] * y[i][k];

	}
	mol.addConstr(c5 <= VehicleCap, "c5" + to_string(constrNum));
	c5 = 0.0;
	constrNum++;
}
cout << "约束个数" << constrNum - 1 << endl;


for (int i = 1; i < 11; i++)
{
	for (j = 1; j < 11; j++)
	{
		if (i != j)
		{
			for (int k = 0; k < VehicleNum; k++)
			{
				mol.addConstr(u[i][k] - u[j][k] + 10 * VehicleCap * x[i][j][k] <= 10 * VehicleCap - Demand[j], "c6" + to_string(i));
			}
		}
	}
}

*/
