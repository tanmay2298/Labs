#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#define MAX_SOURCE_SIZE (0x100000)

//Define height and width of matrices
#define WA 3
#define HA 2
#define WB 3
#define HB 3
#define WC 3
#define HC 2
#define BLOCK_SIZE 1

int main(){
	unsigned int size_A = WA*HA, size_B = WB*HB;
	int *a = malloc(size_A*sizeof(int));
	int *b = malloc(size_B*sizeof(int));

	FILE *fp = fopen("matrixKernel.cl","r");
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
	printf("Enter elements of B\n");
	for(int i = 0; i < size_B; i++)
			scanf("%d",b+i);
	unsigned int size_C = WC*HC;
	int *c = malloc(size_C*sizeof(int));


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
	cl_mem a_mem_obj = clCreateBuffer(context,CL_MEM_READ_ONLY,size_A*sizeof(int),NULL,&ret);
	cl_mem b_mem_obj = clCreateBuffer(context,CL_MEM_READ_ONLY,size_B*sizeof(int),NULL,&ret);
	cl_mem c_mem_obj = clCreateBuffer(context,CL_MEM_WRITE_ONLY,size_C*sizeof(int),NULL,&ret);

	//Copy lists A & B to memory buffers
	ret = clEnqueueWriteBuffer(command_queue,a_mem_obj,CL_TRUE,0,sizeof(int)*size_A,a,0,NULL,NULL);
	ret = clEnqueueWriteBuffer(command_queue,b_mem_obj,CL_TRUE,0,sizeof(int)*size_B,b,0,NULL,NULL);

	//Create program from kernel source
	cl_program program = clCreateProgramWithSource(context,1,(const char**)&source_str,(const size_t *)&source_size,&ret);

	//Build program
	ret = clBuildProgram(program,1,&device_id,NULL,NULL,NULL);

	//Create kernel
	cl_kernel kernel = clCreateKernel(program,"mat_mul",&ret);

	//Set Kernel Args
	int widthA= WA, widthB = WB;
	ret = clSetKernelArg(kernel,0,sizeof(cl_mem),(void *)&a_mem_obj);
	ret = clSetKernelArg(kernel,1,sizeof(cl_mem),(void *)&b_mem_obj);
	ret = clSetKernelArg(kernel,2,sizeof(cl_mem),(void *)&c_mem_obj);
	ret = clSetKernelArg(kernel,3,sizeof(cl_int),(void *)&widthA);
	ret = clSetKernelArg(kernel,4,sizeof(cl_int),(void *)&widthB);

	size_t localWorkSize[2] = {BLOCK_SIZE,BLOCK_SIZE};
	size_t globalWorkSize[2] = {WC,HC};
	ret = clEnqueueNDRangeKernel(command_queue,kernel,2,NULL,globalWorkSize,localWorkSize,0,NULL,NULL);
	clFinish(command_queue);

	ret = clEnqueueReadBuffer(command_queue,c_mem_obj,CL_TRUE,0,sizeof(int)*size_C,c,0,NULL,NULL);
	printf("Matrix C: \n");
	for(int i = 0; i < size_C; i++){
		printf("%d\t",c[i]);
		if((i+1)%WC == 0)
			printf("\n");
	}

	free(source_str);
	clReleaseContext(context);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(command_queue);

}
