#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#define MAX_SOURCE_SIZE (0x100000)
#define BLOCK_SIZE 1

int main(){
	int M,N;
	printf("Enter dimensions of A\n");
	scanf("%d %d",&M,&N);
	unsigned int size_A = M*N;
	int *a = malloc(size_A*sizeof(int));

	FILE *fp = fopen("q1_kernel.cl","r");
	if(!fp){
		fprintf(stderr,"Failed to load kernel\n");
		exit(1);
	}

	char *source_str = malloc(MAX_SOURCE_SIZE);
	size_t source_size = fread(source_str,1,MAX_SOURCE_SIZE,fp);

	fclose(fp);
	printf("Enter elements of A\n");
	for(int i = 0; i < size_A; i++)
		scanf("%d",a+i);
	printf("Initially, Matrix A is: \n");
		for(int i = 0; i < size_A; i++){
			printf("%d\t",a[i]);
			if((i+1)%N == 0)
				printf("\n");
		}


	//Get platform and device info
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices, ret_num_platforms;
	cl_int ret = clGetPlatformIDs(1,&platform_id,&ret_num_platforms);
	ret = clGetDeviceIDs(platform_id,CL_DEVICE_TYPE_GPU,1,&device_id,&ret_num_devices);

	//Create context
	cl_context context = clCreateContext(NULL,1,&device_id,NULL,NULL,&ret);

	//Create cmd queue
	cl_command_queue command_queue = clCreateCommandQueue(context,device_id,NULL,&ret);

	//Create memory buffers
	cl_mem a_mem_obj = clCreateBuffer(context,CL_MEM_READ_WRITE,size_A*sizeof(int),NULL,&ret);

	//Copy list A to memory buffers
	ret = clEnqueueWriteBuffer(command_queue,a_mem_obj,CL_TRUE,0,sizeof(int)*size_A,a,0,NULL,NULL);

	//Create program from kernel source
	cl_program program = clCreateProgramWithSource(context,1,(const char**)&source_str,(const size_t *)&source_size,&ret);

	//Build program
	ret = clBuildProgram(program,1,&device_id,NULL,NULL,NULL);

	//Create kernel
	cl_kernel kernel = clCreateKernel(program,"mat_exp",&ret);

	//Set Kernel Args
	int widthA= N;
	ret = clSetKernelArg(kernel,0,sizeof(cl_mem),(void *)&a_mem_obj);
	ret = clSetKernelArg(kernel,1,sizeof(cl_int),(void *)&widthA);

	size_t localWorkSize[2] = {BLOCK_SIZE,BLOCK_SIZE};
	size_t globalWorkSize[2] = {M,N};
	ret = clEnqueueNDRangeKernel(command_queue,kernel,2,NULL,globalWorkSize,localWorkSize,0,NULL,NULL);
	clFinish(command_queue);

	ret = clEnqueueReadBuffer(command_queue,a_mem_obj,CL_TRUE,0,sizeof(int)*size_A,a,0,NULL,NULL);
	printf("Matrix A is now: \n");
	for(int i = 0; i < size_A; i++){
		printf("%d\t",a[i]);
		if((i+1)%N == 0)
			printf("\n");
	}

	free(source_str);
	clReleaseContext(context);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(command_queue);

}
