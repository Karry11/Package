Deep Binarized Convolutional Neural Network
Inferences over Encrypted Data
====


Description
-----
Deep learning is becoming more and more popular. However it cost large time on calculating privacy data with fully homomorphic  encryption. We propose an effcient and secure way to deal with privacy data by CNN model.
In our model, we define that the client encrypt their privacy by himself. On the cloud, we construct the way to perform homomorphic operations between the client's data and plaintext parameters. After finishing homomorphic calculation on services, it will return a result to the client under encrypted. In the entire process, we can't decrypt privacy data because of without secret key. So it can be proved that client's privacy data won't be leaked.


Require For Experiments
-----
1. In our model, we use TFHE(Fast Fully Homomorphic Encryption Library over the Torus) to encrypt data. You can install TFHE using this document(https://tfhe.github.io/tfhe/)
2. All of our code is written in C++. All models and operations are executed on the PC with i7-6700 CPU and 8G RAM.

Documents Description
-----
Binactive Operation on user's data and server's weight: 0.bmp is an image which be regarded user's data. Secretkey.data is the key to encrypt the privacy data. K_cloud.data and Secret_cloud.data are regared as the inputs on cloud calculation. If you want to run the code, just enter "g++ your-program.cpp -o your-program -ltfhe-spqlios-fma" in the terminal.

Xnor-Hybrid CNN: K_cloud.data and Secret_cloud.data are inputs for Xnor-Hybrid CNN. convolution_new.txt and fullconnection_new.txt are CNN model's parameters. In the terminal, you can enter "g++ main.cpp -o output -ltfhe-spqlios-fma" to run the code. 

Result
-----
Time consumption for XNOR-Hybrid models  

![](https://github.com/Karry11/Package/blob/master/Models.png
)  

Test model accuracy on plaintext and ciphertext  

![](https://github.com/Karry11/Package/blob/master/Comparision.png
)
