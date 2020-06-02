#include <iostream>
#include <math.h>
#include <vector> 

using namespace std;

int main ()
{
//define******************************************************************************************
    int n,m,i,j,k;
    int cities [100];
    int cityWeight [100];
    int pairs [101][51][2];
    int location [101];
//operation***************************************************************************************

    struct operation
{
    int type; // 1 : travel , 2 : visit
    int day;
    int family;
    int destination;
};

//void printOperation(operation op){
//    cout<<op.type<<" "<<op.day<<" "<<op.family<<" "<<op.destination;
//}

//cin**********************************************************************************************
    cin >> n>> m;
    for(int i=0; i<m; i++){
        int source,destination,roadWeight;
        cin >> source >> destination >> roadWeight;
    }

    for(int i=0; i<n; i++){
        cin>>cityWeight[i];
    }
    for (i=0 ; i<n ; i++)
    {
        cities [i] = i+1;
        location [i+1] = i+1;
    }
//even**************************************************************************************************
    if (n % 2 == 0)
    {
        int PairsPointer;
        for (i=0 ; i< (n/2) ; i++)
        {
            pairs [i][0][0] = cities [i];
            pairs [i][0][1] = cities [i+1];
            int firsttemp0 = i , firsttemp1 = i+1;
            PairsPointer = 0;
            for (j=i-1 ; j>=0 ; j--)
            {
                if (firsttemp1+1==n)
                {

                }
                else
                {
                pairs [i][PairsPointer+1][0] = cities [firsttemp0-1];
                pairs [i][PairsPointer+1][1] = cities [firsttemp1+1];
                firsttemp0 = firsttemp0-1;
                firsttemp1 = firsttemp1+1;
                PairsPointer++;
                }
            }
            int secondtemp0 = firsttemp1, secondtemp1 = n;
            for (j=firsttemp1+1 ; j<= firsttemp1 + ((n-firsttemp1)/2) ; j++)
            {
                pairs [i][PairsPointer+1][0] = cities [secondtemp0+1];
                pairs [i][PairsPointer+1][1] = cities [secondtemp1-1];
                secondtemp0 = secondtemp0+1;
                secondtemp1 = secondtemp1-1;
                PairsPointer++;
            }
        }
        if (n>5){
        int pointer = PairsPointer+1;
        for (i=0 ; i< (n/2) ; i++)
        {
            pairs [i+pointer][0][0] = cities [i];
            pairs [i+pointer][0][1] = cities [i+2];
            int firsttemp0 = i , firsttemp1 = i+2;
            PairsPointer = 0;
            for (j=i-1 ; j>=0 ; j--)
            {
                if (firsttemp1+1==n)
                {

                }
                else
                {
                pairs [i+pointer][PairsPointer+1][0] = cities [firsttemp0-1];
                pairs [i+pointer][PairsPointer+1][1] = cities [firsttemp1+1];
                firsttemp0 = firsttemp0-1;
                firsttemp1 = firsttemp1+1;
                PairsPointer++;
                }
            }
            int secondtemp0 = firsttemp1, secondtemp1 = n;
            for (j=firsttemp1+1 ; j<= firsttemp1 + ((n-firsttemp1-1)/2) ; j++)
            {
                pairs [i+pointer][PairsPointer+1][0] = cities [secondtemp0+1];
                pairs [i+pointer][PairsPointer+1][1] = cities [secondtemp1-1];
                secondtemp0 = secondtemp0+1;
                secondtemp1 = secondtemp1-1;
                PairsPointer++;
            }
        }
        }
        else if (n==4)
        {
            pairs [2][0][0] = 1;
            pairs [2][0][1] = 3;
            pairs [2][1][0] = 2;
            pairs [2][1][1] = 4;
        }
    }
//odd**********************************************************************************************************
    else
    {
        int PairsPointer;
        for (i=0 ; i< floor(n/2) + 1 ; i++)
        {
            pairs [i][0][0] = cities [i];
            pairs [i][0][1] = cities [i+1];
            int firsttemp0 = i , firsttemp1 = i+1;
            PairsPointer = 0;
            for (j=i-1 ; j>=0 ; j--)
            {
                pairs [i][PairsPointer+1][0] = cities [firsttemp0-1];
                pairs [i][PairsPointer+1][1] = cities [firsttemp1+1];
                firsttemp0 = firsttemp0-1;
                firsttemp1 = firsttemp1+1;
                if (pairs [i][PairsPointer+1][1]>n || pairs [i][PairsPointer+1][1] <= 0)
                {
                    pairs [i][PairsPointer+1][0] = 0 ;
                    pairs [i][PairsPointer+1][1] = 0 ;
                }
                PairsPointer++;
                
                
            }
            int secondtemp0 = firsttemp1, secondtemp1 = n;
            for (j=firsttemp1+1 ; j<= firsttemp1 + ((n-firsttemp1)/2)-1 ; j++)
            {
                pairs [i][PairsPointer+1][0] = cities [secondtemp0+1];
                pairs [i][PairsPointer+1][1] = cities [secondtemp1-1];
                secondtemp0 = secondtemp0+1;
                secondtemp1 = secondtemp1-1;
                PairsPointer++;
            }
        }
        int pointer = PairsPointer+1;
        for (i=0 ; i< (n/2) ; i++)
        {
            pairs [i+pointer][0][0] = cities [i];
            pairs [i+pointer][0][1] = cities [i+2];
            int firsttemp0 = i , firsttemp1 = i+2;
            PairsPointer = 0;
            for (j=i-1 ; j>=0 ; j--)
            {
                if (firsttemp1+1==n)
                {

                }
                else
                {
                pairs [i+pointer][PairsPointer+1][0] = cities [firsttemp0-1];
                pairs [i+pointer][PairsPointer+1][1] = cities [firsttemp1+1];
                firsttemp0 = firsttemp0-1;
                firsttemp1 = firsttemp1+1;
                PairsPointer++;
                }
            }
            int secondtemp0 = firsttemp1, secondtemp1 = n;
            for (j=firsttemp1+1 ; j<= firsttemp1 + ((n-firsttemp1-1)/2) ; j++)
            {
                pairs [i+pointer][PairsPointer+1][0] = cities [secondtemp0+1];
                pairs [i+pointer][PairsPointer+1][1] = cities [secondtemp1-1];
                secondtemp0 = secondtemp0+1;
                secondtemp1 = secondtemp1-1;
                PairsPointer++;
            }
        }
    }
    

//OP**********************************************************************************************************
vector<operation> operations;

int daycounter=1;
operation op;
    for (i=0 ; pairs[i][0][0] != 0; i++)
    {
        for (j=0 ; pairs [i][j][0] != 0 ; j++)
        {
            if (location [pairs [i][j][1]] != pairs [i][j][1])
            {
                op.type = 1;
                op.day = daycounter;
                op.family = pairs [i][j][1] ;
                op.destination = pairs [i][j][1];
                location [op.family] = op.destination;
                operations.push_back (op);
            }
            //for (k=0 ; pairs [i-1][k][0] != 0 ; k++)
            //{
                //if (pairs [i-1][k][1] == pairs [i][j][1])
                //{
                //    op.type = 1;
                //    op.day = daycounter;
                //    op.family = pairs [i][j][1] ;
                //    op.destination = pairs [i][j][1];
                //    operations.push_back (op);
                    //cout << "1 " << daycounter << " " << pairs [i][j][1] << " " << pairs [i][j][1] << endl ;
                //}
            //}
            op.type = 1;
            op.day = daycounter;
            op.family = pairs [i][j][0] ;
            op.destination = pairs [i][j][1];
            location [op.family] = op.destination;
            operations.push_back (op);
            //cout << "1 " << daycounter << " " << pairs [i][j][0] << " " << pairs [i][j][1] << endl ;
            op.type = 2;
            op.day = daycounter;
            op.family = pairs [i][j][0] ;
            op.destination = pairs [i][j][1];
            location [op.family] = op.destination;
            operations.push_back (op);
            //cout << "2 " << daycounter << " " << pairs [i][j][0] << " " << pairs [i][j][1] << endl ;
        }
        daycounter++;
        for (j=0 ; pairs [i][j][0] != 0 ; j++)
        {
            op.type = 1;
            op.day = daycounter;
            op.family = pairs [i][j][1] ;
            op.destination = pairs [i][j][0];
            location [op.family] = op.destination;
            operations.push_back (op);
            //cout << "1 " << daycounter << " " << pairs [i][j][1] << " " << pairs [i][j][0] << endl ;
            op.type = 1;
            op.day = daycounter;
            op.family = pairs [i][j][0] ;
            op.destination = pairs [i][j][0];
            location [op.family] = op.destination;
            operations.push_back (op);
            //cout << "1 " << daycounter << " " << pairs [i][j][0] << " " << pairs [i][j][0] << endl ;
            op.type = 2;
            op.day = daycounter;
            op.family = pairs [i][j][1] ;
            op.destination = pairs [i][j][0];
            location [op.family] = op.destination;
            operations.push_back (op);
            //cout << "2 " << daycounter << " " << pairs [i][j][1] << " " << pairs [i][j][0] << endl ;
        } 
        daycounter++;
    }

//cout***********************************************************************************************************

    cout<<operations.size()<<endl;
	for (int i = 0; i < operations.size(); ++i)
	{
        op = operations [i];
		cout<<op.type<<" "<<op.day<<" "<<op.family<<" "<<op.destination;
		cout<<endl;
	}


    return 0;
}