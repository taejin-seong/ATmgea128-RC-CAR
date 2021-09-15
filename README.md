# 블루투스 통신 기반의 ATmgea128 MCU를 사용한 <br> MPU-6050, 조이스틱, 초음파센서 기반 자율주행 원격 RC카
<br>2020년 3학년 1학기 프로젝트 <br><br> 개발환경: <b> 　atmel studio6.2 또는 atmel studio7.0 <b> <br><br>
 기본적으로 ATmega128의 페리페럴 및 MCU 통신프로토콜 등 전반전인 학습을 바탕으로 사용되는 센서모듈의 원리와 제공되는 데이터시트를 통해 학습을 진행하며 프로젝트 진행하였음. <br>
 <br>
 프로젝트중 먼저 HW 구상 및 SW 알고리즘을 생각하고 HW/SW에서 발생하는 문제들을 오실로스코프로 확인해가면서 수정하거나, 깃허브 등의 예제를 참고하여 수정해 나가는 식으로 진행 
<br>
<br>
<br>
<br>
## 1. 구상도
  ### 컨트롤러 부 
![1](https://user-images.githubusercontent.com/70312248/105469609-dff68380-5cdb-11eb-8cd7-9336fde16279.png)

 &nbsp;
  ### RC카 부
![2](https://user-images.githubusercontent.com/70312248/105470071-70cd5f00-5cdc-11eb-81fa-1d385a5710d4.png)##

 &nbsp;
  ### 동작 과정
![image](https://user-images.githubusercontent.com/70312248/105577404-bf522a80-5dbc-11eb-8789-327aba793307.png)

<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>

## 2. 회로도 
  ### 컨트롤러 부 (기본 회로및 센서 모듈 결선)
  ![2-1](https://user-images.githubusercontent.com/70312248/105470596-197bbe80-5cdd-11eb-8243-06489dc3aa7d.png)<hr>
  ### 기본회로부
  ![2-2](https://user-images.githubusercontent.com/70312248/105470606-1d0f4580-5cdd-11eb-86d7-db811fc3ee9d.png)<hr>
  ### 모듈 연결 회로부 (아래 표 참고)
  ![2-3](https://user-images.githubusercontent.com/70312248/105470612-1e407280-5cdd-11eb-8fdc-b70d195e8a72.png)<hr>
  <br>
  <br>
   ![image](https://user-images.githubusercontent.com/70312248/105577477-40112680-5dbd-11eb-83e2-0aa212b21fa0.png)
<br>
<br>
<br>
<br>
<br>
   ### RC카 부 (기본 회로및 센서 모듈 결선)
  ![1-1](https://user-images.githubusercontent.com/70312248/105471304-edad0880-5cdd-11eb-8fe6-400919ac7ad0.png)<hr>
  ### 기본회로
  ![1-2](https://user-images.githubusercontent.com/70312248/105471310-eede3580-5cdd-11eb-896a-25cef1d87ffa.png) <hr>
  ### 모듈 연결 회로부 (아래 표 참고)
  ![1-3](https://user-images.githubusercontent.com/70312248/105471318-ef76cc00-5cdd-11eb-91bf-04787c537d8a.png)<hr>
  <br>
  <br>
  <br>
   ### ![image](https://user-images.githubusercontent.com/70312248/105577453-135d0f00-5dbd-11eb-853a-f909fd1bfd0e.png) <br>
<br>
<br>
<br>
<br>
<br>
   
## 3. 작품 시연
   [![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/TKuDrxt8rrI/0.jpg)](https://www.youtube.com/watch?v=TKuDrxt8rrI) 
     &nbsp;
     &nbsp;
     <br>ο 각 해당 기능 유튜브 설명란 참고
 <br>
 <br>
 <br>
 <img width="50%" src="https://user-images.githubusercontent.com/70312248/133422183-0759e38e-b749-4988-8bd1-45d6dbf6c26b.gif"/><br>
 &nbsp;&nbsp;ο IMU 모드 주행 영상 (배속효과X)



&nbsp;
&nbsp;
<br>
<br>
<br>
<br>


## 4. 보완해야할 사항
   1. 모터에 들어가는 전압이 불안정한 것을 확인하였고 알카라인 건전지 대신 Lipo 배터리로 교체가 필요.
  
   2. 예산 상의 문제로 저급 DC모터를 사용하여서 모터의 토크와 모터의 RPM이 원활하지 않음 조금더 고급사양의 모터로 교체 필요.
   
   3. 전체적으로 코드를 수정하고 다듬었으나, 수정된 파일이 소실되어 시간이 되면 전체적으로 다시 한번 다듬기 필요.

   4. 외형적으로 기구설계를 다듬어서 케이스를 씌우는 등의 보기 좋게 꾸며볼 필요가 있음.
   
   5. PCB 기판의 사이즈를 좀더 소형화시켜볼 시도가 필요.
   
   6. 구조체 포인터등을 활용하여 좀더 고급스러운 코딩으로 메모리를 적게 할당하는 방안으로 수정할 필요.
   


<br>
<br>
<br>
<br>

## 5. 참고 사이트
https://jdselectron.tistory.com/15?category=730344
https://m.blog.naver.com/PostView.nhn?blogId=speedprinse&logNo=221187451092&proxyReferer=https:%2F%2Fwww.google.com%2F
