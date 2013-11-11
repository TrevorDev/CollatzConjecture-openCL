__kernel void square(__global int* input,__global int* output,const unsigned int count){
	int cur;
	int counter=0;
	int i = get_global_id(0);
	if(i < count) {
		cur = input[i];
		while(cur!=1){
			counter++;
			if(cur%2==0){
				cur=cur/2;
			}else{
				cur=(cur*3)+1;
			}
			if(counter>25){
				break;
			}
		}
		output[i] = counter;
	}
}