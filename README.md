## ATmgea128 MCU를 사용한 초음파센서 기반 자율주행 및 BLE를 통한 MPU-6050, 조이스틱 원격 제어 RC카

<br>

　　**2020년 3학년 1학기 프로젝트** <br>
> 
> * 기본적으로 ATmega128의 페리페럴 및 MCU 통신프로토콜 등 전반전인 학습을 바탕으로 사용되는 센서모듈의 원리와 제공되는 데이터시트를 통해 학습을 진행하며 프로젝트 진행. <br>
>
> * 프로젝트중 먼저 HW 구상 및 SW 알고리즘을 생각하고 HW/SW에서 발생하는 문제들을 오실로스코프로 확인해가면서 수정하거나, 깃허브 등의 예제를 참고하여 수정해 나가는 식으로 진행. 

<br><br>

## 개발 환경<br>
### IDE
* Atmel studio 6.2 <br>

<br><br>


## 구상도
### 컨트롤러부  
<p align="center">
   <img src="https://user-images.githubusercontent.com/70312248/105469609-dff68380-5cdb-11eb-8cd7-9336fde16279.png" width="430" height="520"/>  
</p>  
      
<br>

### RC카 부
<p align="center">
   <img src="https://user-images.githubusercontent.com/70312248/105470071-70cd5f00-5cdc-11eb-81fa-1d385a5710d4.png" width="800" height="480"/>  
</p> 

<br>

### 동작 과정
<p align="center">
   <img src="https://user-images.githubusercontent.com/70312248/105577404-bf522a80-5dbc-11eb-8789-327aba793307.png" width="919" height="266"/>  
</p>


<br><br>
<br><br>


## 회로도 
<br>

## 컨트롤러 부
### 기본 회로 및 센서 모듈 결선
<p align="center">  
  <img src="https://user-images.githubusercontent.com/70312248/105470596-197bbe80-5cdd-11eb-8243-06489dc3aa7d.png">   
</p>
<hr>
<br><br>


### 기본회로부
<p align="center">  
  <img src="https://user-images.githubusercontent.com/70312248/105470606-1d0f4580-5cdd-11eb-86d7-db811fc3ee9d.png">   
</p>
<hr>
<br><br>


### 모듈 연결 회로부 (아래 표 참고)
<p align="center">  
  <img src="https://user-images.githubusercontent.com/70312248/105470612-1e407280-5cdd-11eb-8fdc-b70d195e8a72.png">   
</p>

<br>

<p align="center">  
  <img src="https://user-images.githubusercontent.com/70312248/105577477-40112680-5dbd-11eb-83e2-0aa212b21fa0.png">   
</p>

<br><br>
<br><br>




## RC카 부 

### 기본 회로 및 센서 모듈 결선
<p align="center">  
  <img src="https://user-images.githubusercontent.com/70312248/105471304-edad0880-5cdd-11eb-8fe6-400919ac7ad0.png">   
</p>
<hr>
<br><br>


### 기본회로
<p align="center">  
  <img src="https://user-images.githubusercontent.com/70312248/105471310-eede3580-5cdd-11eb-896a-25cef1d87ffa.png">   
</p>
<hr>
<br><br>


### 모듈 연결 회로부 (아래 표 참고)
<p align="center">  
  <img src="https://user-images.githubusercontent.com/70312248/105471318-ef76cc00-5cdd-11eb-91bf-04787c537d8a.png">   
</p>

<br><br><br>

<p align="center">  
  <img src="https://user-images.githubusercontent.com/70312248/105577453-135d0f00-5dbd-11eb-853a-f909fd1bfd0e.png">   
</p>


<br><br>


## 작품 구현 영상
<br>

<div align="center">  
 <b> [ 각 해당 기능 유튜브 설명란 참고 ]</b>  <br><br>
   
   [![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/TKuDrxt8rrI/0.jpg)](https://www.youtube.com/watch?v=TKuDrxt8rrI)   

</div>

<br>


<div align="center">  
 <b> [ IMU 모드 주행 영상 ]</b>  <br><br>
   
<img width="50%"  src="https://user-images.githubusercontent.com/70312248/133422183-0759e38e-b749-4988-8bd1-45d6dbf6c26b.gif">   

</div>

<br><br><br>

## 보완 사항
* 모터에 들어가는 전압이 불안정한 것을 확인하였고 모터의 외부전압에 알카라인 건전지 대신 Lipo 배터리로 교체가 필요.
* 예산상의 문제로 저급 DC모터를 사용하여 모터의 토크와 RPM이 원활하지 않음 따라서 적절한 사양의 DC모터로 교체할 필요가 있음.
* 사용자의 편리성 및 디자인 등을 고려하여 3D 프린터를 이용한 기구설계 및 케이스 제작을 고려할 필요가 있음.
* PCB 기판의 사이즈를 좀더 소형화시켜볼 시도가 필요가 있음.
* 구조체 포인터 등을 활용하여 유지보수가 쉬우면서 체계적이도록 프로그램을 작성할 필요가 있음.
* 코드 최적화를 통한 메모리의 효율을 향상 시켜 볼 필요가 있음.    


<br><br>

## 참고 자료
https://jdselectron.tistory.com/15?category=730344<br>
https://m.blog.naver.com/PostView.nhn?blogId=speedprinse&logNo=221187451092&proxyReferer=https:%2F%2Fwww.google.com%2F
