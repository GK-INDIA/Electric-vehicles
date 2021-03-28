#include<stdio.h>
#include<bits/stdc++.h>
#include<conio.h>
#define INFINITY 9999
#define MAX 1000

using namespace std;
 
void dijkstra(int G[MAX][MAX],int n,int startnode,int end,int count,vector<vector<int>>&v,vector<float>&ans);

void update_time(int G[MAX][MAX],vector<vector<int>>&v,vector<vector<float>>&start_time,
                 vector<vector<float>>&end_time,vector<float>&ans,vector<int>&speed,
                 vector<int>&max_capacity,vector<int>&discharge,vector<int>&charging,
                 vector<int>&initial_battery,int count)
{
    ans[count]= (float)ans[count]/speed[count];
    start_time[count][0]=0;
    end_time[count][0]=0;
    int battery_status = initial_battery[count];

    for(int i=0;i<v[count].size()-1;i++)
    {
        float time1;
        float time2;
        
        time1 = (float)G[v[count][i]][v[count][i+1]]/speed[count];
        time2 = (float)battery_status/discharge[count];
        
        start_time[count][i+1] = end_time[count][i] + time1;
        end_time[count][i+1] = start_time[count][i+1];
        
        if(time1>time2)
        {
            float gap_time = (((float)G[v[count][i]][v[count][i+1]]*discharge[count])/speed[count] - battery_status)*charging[count];
            battery_status = ((float)G[v[count][i]][v[count][i+1]]*discharge[count])/speed[count];
            start_time[count][i+1]+=gap_time;
            end_time[count][i]+=gap_time;
            end_time[count][i+1]+=gap_time;
            ans[count]+=gap_time;
        }
        battery_status-=time1*discharge[count];
        
        if(i == v[count].size()-1)
        {
            end_time[count][i+1] = start_time[count][i+1];
        }
        
    }
    
    return;
}
 
int main()
{
	int G[MAX][MAX],i,j,n,u,end,k;          // G represents the adjacency matrix, u represents the Starting City
	                                        // end represents the Destination City
	printf("Enter no. of vertices:");      
	scanf("%d",&n);
	printf("Enter the adjacency matrix:\n");
	
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			scanf("%d",&G[i][j]);
			
	printf("Enter no. of Electric Vehicles:");
	cin>>k;
	cout<<"\n";
	vector<vector<int>> v;                                          // v is a 2D array to store the optimum path visited by each Vehicle
	vector<vector<float>> start_time(n,vector<float>(1000,0));      // start_time is an array to store the time of entry to each city in the optimal route of each vehicle 
	vector<vector<float>> end_time(n,vector<float>(1000,0));        // end_time is an array to store the time of exit from each city in the optimal route of each vehicle 
	vector<float> ans;                                              // ans is an array to store the optimum time required by each vehicle to go from its source to destination
	
	vector<int> speed;                                              // speed is an array to store the average speed of each vehicle
	vector<int> max_capacity;                                       // max_capacity is an array to store the maximum capacity of battery of each vehicle
	vector<int> discharge;                                          // discharge is an array to store the discharge rate of battery of each vehicle
	vector<int> charging;                                           // charging is an array to store the charging rate of battery of each vehicle
	vector<int> initial_battery;                                    // initial_battery is an array to store the initial battery capacity of each vehicle
	int count = 0;
	while(k--)
	{
	    int a,b,c,max_cap,sr;
	    
	    printf("Electric Vehicle %d:",count+1);
	    cout<<"\n";
	    
	    printf("Enter initial battery status, charging rate for battery, discharging rate of battery :");
	    cin>>a>>b>>c;
	    initial_battery.push_back(a);
	    charging.push_back(b);
	    discharge.push_back(c);
	    
	    printf("Enter the Maximum Battery Capacity :");
	    cin>>max_cap;
	    max_capacity.push_back(max_cap);
	    
	    printf("Enter the Average Travelling Speed:");
	    cin>>sr;
	    speed.push_back(sr);
	    
	    
	    printf("Enter the starting node:");
	    scanf("%d",&u);
	    
	    
	    printf("Enter the destination node:");
	    cin>>end;
	    
	    dijkstra(G,n,u,end,count,v,ans);        // Function to finad and store the optimum route followed by each vehicle
	    update_time(G,v,start_time,end_time,ans,speed,max_capacity,discharge,charging,initial_battery,count);    //Function to store the entry time and exit time of a vehicle of nodes that it visits in its optimal path
	    count++;
	    cout<<"\n";
	}
	
	printf("The optimum time required is ");
	cout<<*max_element(ans.begin(),ans.end())<<" ";  // print the optimal time 
	printf("(in seconds)");
	cout<<"\n";
	
	return 0;
}
 
void dijkstra(int G[MAX][MAX],int n,int startnode,int end,int cnt,vector<vector<int>>&v,vector<float>&ans)
{
 	int cost[MAX][MAX],distance[MAX],pred[MAX];
	int visited[MAX],count,mindistance,nextnode,i,j;
	
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			if(G[i][j]==0)
				cost[i][j]=INFINITY;
			else
				cost[i][j]=G[i][j];
	
	for(i=0;i<n;i++)
	{
		distance[i]=cost[startnode][i];
		pred[i]=startnode;
		visited[i]=0;
	}
	
	distance[startnode]=0;
	visited[startnode]=1;
	count=1;
	
	while(count<n-1)
	{
		mindistance=INFINITY;
		
		for(i=0;i<n;i++)
			if(distance[i]<mindistance&&!visited[i])
			{
				mindistance=distance[i];
				nextnode=i;
			}
			
			visited[nextnode]=1;
			for(i=0;i<n;i++)
				if(!visited[i])
					if(mindistance+cost[nextnode][i]<distance[i])
					{
						distance[i]=mindistance+cost[nextnode][i];
						pred[i]=nextnode;
					}
		count++;
	}
    vector<int> temp;
    i = end;

		if(i!=startnode)
		{
			temp.push_back(i);
			
			j=i;
			do
			{
				j=pred[j];
				temp.push_back(j);
			}while(j!=startnode);
	}
	ans.push_back(distance[i]);
	reverse(temp.begin(),temp.end());
	v.push_back(temp);                  // stores the optimal path of that vehicle in 2D array v
	return;
}
