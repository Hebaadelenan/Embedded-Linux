#include <stdio.h>

int binary_search(int arr[],int size ,int target)
{
int start = 0;
int end = size-1;
int mid ;
while(start<=end)
{
mid = ( start + end )/2;
if(arr[mid]==target)
{
return mid;
}
else if(arr[mid]<target)
{
mid ++;
start = mid;
}
else if(arr[mid]>target)
{
mid --;
end = mid;
}
}
return -1;
}


int main(void)
{
int arr[] = {2,5,8,10,12};
int result = binary_search(arr,5,8);
printf("%d ",result);

return 0;
}

/*
the file is executed and generates the output 
because /usr/local/bin is in the "$PATH" 
so the file can be executed anywhere
*/