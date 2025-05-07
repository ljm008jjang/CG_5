Compilation:
Sequence of rendering.
make MVP matrix, then calculate position of each triangle's world position and screen position
and check is in frustum, and back-face culling, finally do raterizing.

![image](https://github.com/user-attachments/assets/b9f9e810-8124-4f96-bde4-f32c4e0438ef)

insert x,y,z to gVertexBuffer

![image](https://github.com/user-attachments/assets/7a8a629c-e4f5-4b81-9c12-f08e58bbe67d)

code of make view, projection matrix and FrustrumCheck

![image](https://github.com/user-attachments/assets/97caf3c2-e55d-4603-9e42-4ab8a390654a)

code of calc triangle's world position and screen position.

![image](https://github.com/user-attachments/assets/67ea3c1b-7ddc-421a-a12c-cbc9aa13c03b)

The code is based on the course materials, but for the sake of code convenience, different code with the same meaning has been used in some parts.
For a detailed explanation, please refer to the comments.
![image](https://github.com/user-attachments/assets/3abb0ec2-9e53-4589-ab14-5dc5f50ed0b2)



Run instructions:
Download files and run OpenglViewer.sln

![image](https://github.com/user-attachments/assets/00a6fa01-53a9-4d9e-a088-0ffd1c6aa84d)


Play "Local Windows Debugger"

![image](https://github.com/user-attachments/assets/9a435ea5-9ba9-46e0-9fa5-d4852fcf01e6)

Result Picture:
result picture included in root folder

![result](https://github.com/user-attachments/assets/f4684078-b21d-4a0d-8a4f-d62d012812ff)


