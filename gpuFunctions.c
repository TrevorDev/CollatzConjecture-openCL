__kernel void allToOne(__global int* input,__global int* output,const unsigned int count){
	int cur;
	int counter=0;
	int i = get_global_id(0);
	if(i < count) {
		cur = input[i];
		if(cur==-1){
			output[i] = -1;
		}else{
			while(cur!=1){
				counter++;
				if(cur%2==0){
					cur=cur/2;
				}else{
					cur=(cur*3)+1;
				}
				if(counter>100000){
					break;
				}
			}
			output[i] = counter;
		}
		
	}
}

__kernel void oneToAll(__global int* input,__global int* output,const unsigned int count){
	int cur;
	int counter=0;
	int i = get_global_id(0);
	if(i < count) {
		cur = input[i];
		if(cur==-1){
			output[i] = -1;
		}else{
			while(cur!=1){
				counter++;
				if(cur%2==0){
					cur=cur/2;
				}else{
					cur=(cur*3)+1;
				}
				if(counter>100000){
					break;
				}
			}
			output[i] = counter;
		}
		
	}
}