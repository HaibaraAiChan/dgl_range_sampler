###image `ubuntu_22.04_CUDA12.1_py3.10_DGL_source_modified_sampler` use the modified dgl sampler      ###

# Install DGL from source
cd
git clone --recurse-submodules https://github.com/dmlc/dgl.git
cd dgl/
du -h     1.1GB


# CUDA build
mkdir build
cd build
cmake -DUSE_CUDA=ON ..
make -j120

cd ../python
sudo python3 setup.py install
python setup.py build_ext --inplace

alias python='python3'
source ~/.bashrc

Then Modify 10 files     

10 files are different with the original dgl  
~~~

`Dgl/include/dgl/random.h​`  
   &nbsp; `def RangeInt(lower, upper)​`

`Dgl/include/dgl/aten/csr.h​`   
   &nbsp; CSRRowWiseSampling( range= true)​  

`Dgl/include/dgl/sampling/neighbor.h​`    
    SampleNeighbors_YSY​  

`Dgl/src/array/array_op.h​​`   
   CSRRowWiseSamplingUniformYSY​  

`Dgl/src/array/cuda/rowwise_sampling.cu​`   
   CSRRowWiseSamplingUniformYSY​

`Dgl/python/dgl/sampling/neighbor.py​`   
    def sample_neighbors_range()​    
        DGLGraph.sample_neighbors_range = utils.alias_func(sample_neighbors_range)​

`Dgl/src/graph/sampling/neighbor/neighbor.cc​`   
     DGL_REGISTER_GLOBAL("sampling.neighbor._CAPI_DGLSampleNeighbors_Range")​

`Dgl/src/array/cpu/rowwise_sampling.cc​`   
     GetSamplingYSYRangePickFn()​

`Dgl/src/array/array.cc​`     
    CSRRowWiseSamplingYSY​      

​'Dgl/src/random/cpu/choice.cc'    ​
   YSYChoice​
~~~
Then :    

cd dgl/   
sudo rm -r build/   
mkdir build   
cd build   
cmake -DUSE_CUDA=ON ..  
make -j120   

cd ../python   
sudo python3 setup.py install   


`Dgl/src/random/cpu/choice.cc​`       ​   
  YSYChoice​


