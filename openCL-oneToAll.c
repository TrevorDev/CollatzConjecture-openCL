#include <CL/cl.h>
#include "dlxlib.h"
#define DATA_SIZE (524288)

int depth = 1;
int rep = 1;

int parseArgs(int argc, char * argv[]){
    int repSet = 0;
    int depthSet = 0;
    for(int i=0;i<argc;i++){
        int num = atoi(argv[i]+1);
        switch( argv[i][0] ) 
        {
        case 'd':
            depth=num;
            depthSet=1;
            break;
        case 'r':
            rep=num;
            repSet=1;
            break;
        default :
            break;
        }
    }
    if(!depthSet || !repSet || rep<=0||depth<0||depth>19){
        printf("Invalid arguments, expected 2 args:\n");
        printf("r - number of repititions greater than 0(to increase runtime)\n");
        printf("d - depth of reverse collatz tree to print out between 1 and 19\n");
        printf("Example execution ./openCL-allToOne r1 d8 > output.txt\n");
        return 1;
    }
    return 0;
}

void initData(int data[]){
    for(int i = 0; i < DATA_SIZE; i++) {
        if(i==0){
            data[i] = 1;
        }else{
            data[i] = 0;
        }
    }
}

int main(int argc, char *argv[]){
    cl_uint numPlatforms;
    cl_platform_id* clSelectedPlatformID = NULL;  
	int err;                            // error code returned from api calls
      
    int data[DATA_SIZE];              // original data set given to device
    int results[DATA_SIZE];           // results returned from device
    size_t global;                      // global domain size for our calculation
    size_t local;                       // local domain size for our calculation
 
    cl_device_id device_id;              
    cl_context context;                 
    cl_command_queue commands;          
    cl_program program;                 
    cl_kernel kernel;                   
    
    cl_mem input;                       // device memory used for the input array
    cl_mem output;                      // device memory used for the output array
    
    if(parseArgs(argc, argv)){
        return 0;
    }
    //print("%d %d %d",arraySize,rep,*seed);
    // Fill our data set with random int values
    unsigned int count = DATA_SIZE;
    //set result incase d = 0
    initData(results);

	////////////////////////////////////////////////////////////////////////////////
	 
	// Simple compute kernel which computes the collatz of an input array 
	//
	const char *KernelSource = fileToString("gpuFunctions.c");
    
	//get Platform
	clGetPlatformIDs(0, NULL, &numPlatforms);
	clSelectedPlatformID = (cl_platform_id*)malloc(sizeof(cl_platform_id)*numPlatforms);
    err = clGetPlatformIDs(numPlatforms, clSelectedPlatformID, NULL);

    //get Device
    err = clGetDeviceIDs(clSelectedPlatformID[0], CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
    if (err != CL_SUCCESS)
    {
    	printf("Error: Failed to create a device group!\n");
        return EXIT_FAILURE;
    }

    //create context
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    if (!context)
    {
        printf("Error: Failed to create a compute context!\n");
        return EXIT_FAILURE;
    }

     // Create a command commands
    //
    commands = clCreateCommandQueue(context, device_id, 0, &err);
    if (!commands)
    {
        printf("Error: Failed to create a command commands!\n");
        return EXIT_FAILURE;
    }
 
    // Create the compute program from the source buffer
    //
    program = clCreateProgramWithSource(context, 1, (const char **) & KernelSource, NULL, &err);
    if (!program)
    {
        printf("Error: Failed to create compute program!\n");
        return EXIT_FAILURE;
    }

    // Build the program executable
    //
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];
 
        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        exit(1);
    }
 
    // Create the compute kernel in the program we wish to run
    //
    kernel = clCreateKernel(program, "oneToAll", &err);
    if (!kernel || err != CL_SUCCESS)
    {
        printf("Error: Failed to create compute kernel!\n");
        exit(1);
    }


    // Create the input and output arrays in device memory for our calculation
    input = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(float) * count, NULL, NULL);
    output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * count, NULL, NULL);
    if (!input || !output)
    {
        printf("Error: Failed to allocate device memory!\n");
        exit(1);
    }
    timer t = createTimer();
    for(int k =0;k<rep;k++){
        initData(data);
        for(int i =0;i<depth;i++){
            // Write our data set into the input array in device memory 
            //
            err = clEnqueueWriteBuffer(commands, input, CL_TRUE, 0, sizeof(float) * count, data, 0, NULL, NULL);
            if (err != CL_SUCCESS)
            {
                printf("Error: Failed to write to source array!\n");
                exit(1);
            }
         
            // Set the arguments to our compute kernel
            //
            err = 0;
            err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
            err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
            err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &count);
            if (err != CL_SUCCESS)
            {
                printf("Error: Failed to set kernel arguments! %d\n", err);
                exit(1);
            }
         
            // Get the maximum work group size for executing the kernel on the device
            //
            err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
            if (err != CL_SUCCESS)
            {
                printf("Error: Failed to retrieve kernel work group info! %d\n", err);
                exit(1);
            }
         
            // Execute the kernel over the entire range of our 1d input data set
            // using the maximum number of work group items for this device
            //
            
            global = count;
            err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
            if (err)
            {
                printf("Error: Failed to execute kernel!\n");
                return EXIT_FAILURE;
            }
         
            // Wait for the command commands to get serviced before reading back results
            //
            clFinish(commands);
         
            // Read back the results from the device to verify the output
            //
            err = clEnqueueReadBuffer( commands, output, CL_TRUE, 0, sizeof(float) * count, results, 0, NULL, NULL );  
            if (err != CL_SUCCESS)
            {
                printf("Error: Failed to read output array! %d\n", err);
                exit(1);
            }
            for(int j=0;j<results[0];j++){
                data[j]=results[j];
            }
        }
    }
    double timeEnd = getTime(t);
    
    // Validate our results
    //
    for(int i = 0; i < results[0]; i++)
    {
        if(i==0){
           printf("%d",results[i]);
        }else{
           printf(",%d",results[i]);
        }
    }
    printf("\n");
    

    // Print a brief summary detailing the results
    printf("TIME- %f\n",timeEnd);
    
    // Shutdown and cleanup
    clReleaseMemObject(input);
    clReleaseMemObject(output);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);
	return 0;
}

