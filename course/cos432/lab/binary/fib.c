#include<stdio.h>    
int main(){    
	int n1=0,n2=1,n3,i;    
    printf("\n%d %d",n1,n2);//printing 0 and 1    
    n3=n1+n2;    
	for(i=2;n3<1000;++i){    
      printf(" %d",n3);      
      n3=n1+n2;    
      n1=n2;    
      n2=n3;    
    }  
    return 0;  
}    
