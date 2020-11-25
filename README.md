# Sobel filter on grayscale pgm images

Repo that performs sobel filter on grayscale pgm images for edge detection purposes.
Both binary (P2) and text (P5) formats are supported.

You can find sample pgm images on the links below:
* https://people.sc.fsu.edu/~jburkardt/data/pgma/pgma.html
* https://people.sc.fsu.edu/~jburkardt/data/pgmb/pgmb.html


## Usage
First, say `make` to compile.

Pass in the input and desired output filenames with the flags `x` and `o` respectively. 
If you want to see X and Y gradients of the image, simply specify `--export-gradients` option afterwards. 
```shell script
./main -x [input file] -o [output file] --export-gradients (optional)
```

## Examples
### Original image
<p align="center">
  <img width="50%" src="https://user-images.githubusercontent.com/33388526/98713123-44ba6700-2398-11eb-956f-2d84ba3d0fe1.png">
</p>

### Gradients
X             |  Y
:-------------------------:|:-------------------------:
![](https://user-images.githubusercontent.com/33388526/98713129-4552fd80-2398-11eb-98f3-b940552d0223.png) | ![](https://user-images.githubusercontent.com/33388526/98713132-45eb9400-2398-11eb-9d7c-7213de00d7c4.png)

### Output image
<p align="center">
  <img width="50%" src="https://user-images.githubusercontent.com/33388526/98713134-45eb9400-2398-11eb-8a6e-c15ce57f34ec.png">
</p>


### Original image
<p align="center">
  <img width="40%" src="https://user-images.githubusercontent.com/33388526/98713704-fbb6e280-2398-11eb-84ab-5efe2eec7a2c.png">
</p>

### Output image
<p align="center">
  <img width="40%" src="https://user-images.githubusercontent.com/33388526/98713703-fa85b580-2398-11eb-9377-02ed6e59a8a0.png">
</p>
