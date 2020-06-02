#include <stdio.h>

using namespace std;

int main() {
    int n;
    int m;
    scanf("%d",&n);
    scanf("%d",&m);
    int p[n];
    int edgeBegin[m];
    int edgeEnd[m];
    int edgeCost[m];
    for (int i = 0; i < n; ++i) {
        scanf("%d",&p[i]);
    }
    for (int i = 0; i < m; ++i) {
        scanf("%d",&edgeBegin[i]);
        scanf("%d",&edgeEnd[i]);
        scanf("%d",&edgeCost[i]);
    }
    int day = 1;
    if(n%2==0){
        printf("%d\n", (n-1)*((3*n-6)+5)-(n/2)+1);
        for (int i = 0; i < n-1; ++i) {
            printf("%d %d %d %d\n", 1,day,n,i+1);
            printf("%d %d %d %d\n", 2,day,n,i+1);
            for (int j = 1; j < n/2; ++j) {
                printf("%d %d %d %d\n", 1,day,(i-j+n-1)%(n-1)+1,(i+j+n-1)%(n-1)+1);
                printf("%d %d %d %d\n", 2,day,(i-j+n-1)%(n-1)+1,(i+j+n-1)%(n-1)+1);
            }
            day++;
            printf("%d %d %d %d\n", 1,day,n,n);
            printf("%d %d %d %d\n", 1,day,i+1,n);
            printf("%d %d %d %d\n", 2,day,i+1,n);
            for (int j = 1; j < n/2; ++j) {
                printf("%d %d %d %d\n", 1,day,(i-j+n-1)%(n-1)+1,(i-j+n-1)%(n-1)+1);
                printf("%d %d %d %d\n", 1,day,(i+j+n-1)%(n-1)+1,(i-j+n-1)%(n-1)+1);
                printf("%d %d %d %d\n", 2,day,(i+j+n-1)%(n-1)+1,(i-j+n-1)%(n-1)+1);
            }
            day++;
            if(i!=n-2){
                for (int j = 1; j < n/2; ++j) {
                    printf("%d %d %d %d\n", 1,day,(i+j+n-1)%(n-1)+1,(i+j+n-1)%(n-1)+1);
                }
            }
        }
    }
    else{
        int index = 0;
        for (int i = 1; i < n; ++i) {
            if(p[i]<p[index]){
                index=i;
            }
        }
        printf("%d\n", n*(3*n-3)-((n+1)/2)-1);
        for (int i = 0; i < n; ++i) {
            for (int j = 1; j < (n+1)/2; ++j) {
                if(i!=n-1 || j!=1){
                    printf("%d %d %d %d\n", 1,day,(i+index-j+n)%(n)+1,(i+index+j+n)%(n)+1);
                }
                printf("%d %d %d %d\n", 2,day,(i+index-j+n)%(n)+1,(i+index+j+n)%(n)+1);
            }
            day++;
            for (int j = 1; j < (n+1)/2; ++j) {
                printf("%d %d %d %d\n", 1,day,(i+index-j+n)%(n)+1,(i+index-j+n)%(n)+1);
                printf("%d %d %d %d\n", 1,day,(i+index+j+n)%(n)+1,(i+index-j+n)%(n)+1);
                printf("%d %d %d %d\n", 2,day,(i+index+j+n)%(n)+1,(i+index-j+n)%(n)+1);
            }
            day++;
            if(i!=n-1){
                if(((i+index-1+n)%n)!=index){
                    printf("%d %d %d %d\n", 1,day,(i+index+1+n)%n+1,index+1);
                }
                for (int j = 2; j < (n+1)/2; ++j) {
                    printf("%d %d %d %d\n", 1,day,(i+index+j+n)%(n)+1,(i+index+j+n)%(n)+1);
                }
            }
        }
    }
    return 0;
}