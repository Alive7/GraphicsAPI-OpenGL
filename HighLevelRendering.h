#ifndef HLR_HH
#define HLR_HH

class GPUdata {
public:
	unsigned int VAO;
	unsigned int EBO;
	unsigned int VBO;

	GPUdata(void);

	void sendToGPU(size_t size, float* data);
	void render(size_t num_points);

	~GPUdata(void);
};

#endif