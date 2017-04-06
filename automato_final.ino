struct motor{
  byte fim;  // pino do sensor de fim de curso
  byte mais;  // pino de saida do motor no sentido positivo
  byte menos;  // pino de saida do motor no sentido negativo
  byte sensor;  // pino do sensor contador de pulsos
  int pulsos;  // quantidade de pulsos do motor
  int setPoint;  // setPoint desejado para o motor
  int anterior;  // variavel auxiliar para a contagem dos pulsos
  int binario;  // variavel auxiliar para a contagem dos pulsos 
 }; 

struct automato{
  byte estado0;  // estado de leitura da serial
  byte estado1;  // estado para pegar peça na mesa
  byte estado2;  // estado para colocar peça na esteira
  byte estado3;  // estado para voltar aos setpoint iniciais
  byte estado4;
  byte estado5;
  byte estado6;  
  byte estado7;
  byte estado8;
 };

struct esteira{
  byte mais; // pino que ativa a esteira no sentido positivo 
  byte menos; // pino que ativa a esteira no sentido negativo 
  byte fte_entrada; // pino sensor fotoeletrico de entrada 
  byte fte_meio; // pino sensor fotoeletrico intermediário
  byte fte_saida; // pino sensor fotoelétrico de saída ou
};

automato inicializar = {0,0,0,0,0,0,0,0,0}; //estrutura que indica em qual estado está o processo (sinalizado com 1)



// INICIO INICIALIZACAO VARIAVEIS ROBO ------------------------------------------------------------------------
motor distancia = { 4, // pino fim
                   33, // pino sentido + (frente)
                   34, // pino sentido -
                    5, // pino contador
                    0, // quantidade de pulsos
                    0, // setPoint
                    0, // aux
                    0, // aux
                  }; 

motor altura = { 3,// pino fim
                39, // pino sentido + (sobe)
                40, // pino sentido -
                 7, // pino contador
                 0, // quantidade de pulsos
                 0, // setPoint
                 0, // aux
                 0, // aux
               }; 

motor giro = { 2, // pino fim
              38, // pino sentido + (sentido horario)
              37, // pino sentido -
               9, // pino contador
               0, // quantidade de pulsos
               0, // setPoint
               0, // aux
               0, // aux
             }; 

motor garra = { 6, // pino fim
               35, // pino sentido + (fecha)
               36, // pino sentido -
                8, // pino contador
                0, // quantidade de pulsos
                0, // setPoint
                0, // aux
                0, // aux
              };
              

motor giroeg={ 22, // pino fim
               12, // pino sentido + (sentido horário)
               11, // pino sentido -
               48, // pino contador
                0, // quantidade de pulsos
                0, // setPoint
                0, // aux
                0, // aux
              };              
                          
esteira ete={ 10, // pino motor esteira 
               0, 
              26, // pino sensor fte de baixo
               0, 
              50, // pino sensor fte de cima
            };
          
esteira ets1={ 28, // pino motor esteira ets11
               29, // pino motor esteira ets12 
               51, // pino sensor fte de entrada
                0, // pino sensor fte intermediário
               53, // pino sensor fte de saída
             };
             
esteira ets2={ 30, // pino motor esteira ets2
                0, 
               25, // pino sensor fte de entrada
                0,
               23, // pino sensor fte de saída
             };       
       
esteira eg={  27, // pino motor sentido + (tira de ETE em alto)
              13, // pino motor sentido - 
               0, 
              24, // pino sensor fte intermediário
               0, 
             };              
             
#define fte_mesa 52 // sensor fotoelétrico da mesa

// FIM INICIALIZACAO VARIAVEIS ROBO __________________________________________________________________________

byte permissao = 1;  // permite leitura ou nao da serial

int Evento_final=1;//variavel auxiliar que indica se terminou o processo
int auxiliar, grande, ets11=20, esteira_off, pulsos_ets1=0; //variavel auxiliar utilizada na primeira regulagem da altura do estado 2 


void setup() {
  
  // INICIO CONFIGURACAO VARIAVEIS -----------------------------------------------------------------
  pinMode(distancia.fim,INPUT);
  pinMode(distancia.mais,OUTPUT);
  pinMode(distancia.menos,OUTPUT);
  pinMode(distancia.sensor,INPUT);
  
  pinMode(altura.fim,INPUT);
  pinMode(altura.mais,OUTPUT);
  pinMode(altura.menos,OUTPUT);
  pinMode(altura.sensor,INPUT);
  
  pinMode(giro.fim,INPUT);
  pinMode(giro.mais,OUTPUT);
  pinMode(giro.menos,OUTPUT);
  pinMode(giro.sensor,INPUT);
  
  pinMode(garra.fim,INPUT);
  pinMode(garra.mais,OUTPUT);
  pinMode(garra.menos,OUTPUT);
  pinMode(garra.sensor,INPUT);
  
  pinMode(giroeg.fim,INPUT);
  pinMode(giroeg.mais,OUTPUT);
  pinMode(giroeg.menos,OUTPUT);
  pinMode(giroeg.sensor,INPUT);
  
  pinMode(ete.mais,OUTPUT);
  pinMode(ete.fte_entrada,INPUT);
  pinMode(ete.fte_saida,INPUT);
  
  pinMode(ets1.mais,OUTPUT);
  pinMode(ets1.menos,OUTPUT);
  pinMode(ets1.fte_entrada,INPUT);
  pinMode(ets1.fte_meio,INPUT);
  pinMode(ets1.fte_saida,INPUT);
  
  pinMode(ets2.mais,OUTPUT);
  pinMode(ets2.fte_entrada,INPUT);
  pinMode(ets2.fte_saida,INPUT);
  
  pinMode(eg.mais,OUTPUT);
  pinMode(eg.menos,OUTPUT);
  pinMode(eg.fte_meio,INPUT);
    
 // FIM CONFIGURACAO VARIAVEIS ______________________________________________________________________
  
  Serial.begin(9600);
  
}

void loop() {
  
  //********************************************************************* ESTADO 0: COLOCA O MANIPULADOR NA POSIÇÃO INCIAL
     
                     
  if(Evento_final == 1){  // se ja se chegou ao ultimo setPoint definido
   
  auxiliar=0;
  esteira_off=0;
  grande= 3;
  
  
  delay(500);
      digitalWrite(ets1.menos,0);
      digitalWrite(ets1.mais,0);
      digitalWrite(ets2.mais,0);
         
    
  if (!digitalRead(fte_mesa) && inicializar.estado0==0  //Inicia todo o processo apenas se houver uma peça na mesa
  && inicializar.estado1==0 && inicializar.estado2==0 
  && inicializar.estado3==0 && inicializar.estado4==0 
  && inicializar.estado5==0   && inicializar.estado6==0 
  && inicializar.estado7==0 && inicializar.estado8==0){  
    
        inicializar = {0,1,0,0,0,0,0,0,0};
        
        Evento_final=0;
        //permissao = 0;
        //Serial.println("Iniciando processo");
        digitalWrite(ets1.menos,0);
        digitalWrite(ets1.mais,0);
        digitalWrite(ets2.mais,0);
        
      // INICIO DE CORREÇÃO DE POSICIONAMENTO, CASO ALGUM DOS MOVIMENTOS NÃO ESTEJA NO FIM DE CURSO --------------------------------------      
        if (digitalRead(altura.fim) == 0){
          while(digitalRead(altura.fim) != 1){//sobe ate fdc
          digitalWrite (altura.mais, 1);
          digitalWrite (altura.menos, 0); 
          }
          digitalWrite (altura.mais, 0);
          digitalWrite (altura.menos, 0);
          
        }
        
        if(digitalRead(giro.fim) == 0){
          
          while(digitalRead(giro.fim) != 1){//gira sentido horario até fdc
          digitalWrite (giro.menos, 1);
          digitalWrite (giro.mais, 0); 
           }
          digitalWrite (giro.mais, 0);
          digitalWrite (giro.menos, 0);
          
        }        
       
        if(digitalRead(garra.fim) == 0){
          
          while(digitalRead(garra.fim) != 1){//abre ate fdc
          digitalWrite (garra.mais, 0);
          digitalWrite (garra.menos, 1); 
          }
          digitalWrite (garra.mais, 0);
          digitalWrite (garra.menos, 0);           
        
        }
        
        if(digitalRead(distancia.fim) == 0){
   
         while(digitalRead(distancia.fim) != 1){//para trás ate fdc
          digitalWrite (distancia.mais, 0);
          digitalWrite (distancia.menos, 1); 
          }
          digitalWrite (distancia.mais, 0);
          digitalWrite (distancia.menos, 0);      
           
        } 
     // FIM DE CORREÇÃO DE POSICIONAMENTO DO MANIPULADOR _________________________________________________________________________ 
        
     } 
    
    else if (digitalRead(fte_mesa)){
     
      digitalWrite (distancia.mais, 0);
      digitalWrite (distancia.menos, 0); 
      digitalWrite (garra.mais, 0);
      digitalWrite (garra.menos, 0);
      digitalWrite (giro.mais, 0);
      digitalWrite (giro.menos, 0);
      digitalWrite (altura.mais, 0);
      digitalWrite (altura.menos, 0);
      digitalWrite(ets1.menos,0);
      digitalWrite(ets1.mais,0);
      digitalWrite(ets2.mais,0);
      digitalWrite(ete.mais,1);    
    
    } 
   //Serial.println("ESTADO 0");
   delay(400);
   }
  
  
while (Evento_final == 0){
  
      
      
      
     
   // ********************************************************************** ESTADO1: PEGAR PEÇA NA MESA
  
  while((inicializar.estado1 == 1)){
    
    Serial.print(giro.pulsos);
      Serial.print(" ");
      Serial.print(distancia.pulsos);
      Serial.print(" ");
      Serial.print(altura.pulsos);
      Serial.print(" ");  
      Serial.println(garra.pulsos);  
   
     digitalWrite(ets1.menos,0);
      digitalWrite(ets1.mais,0);
      digitalWrite(ets2.mais,0);
   
    altura.setPoint = 145; 
    distancia.setPoint = 160;
    giro.setPoint = 52;
    garra.setPoint = 20;
    //Serial.println("ESTADO 2, PEGANDO A PECA");
    
 // INICIO VERIFICACAO POSICAO GIRO DO ESTADO 1 -------------------------------------------------------------
    if(giro.pulsos == giro.setPoint){
     digitalWrite(giro.mais,0);
     digitalWrite(giro.menos,0);
    }
    if(giro.pulsos != giro.setPoint){
     digitalWrite(giro.mais,1);
     digitalWrite(giro.menos,0);
     
     giro.binario = digitalRead(giro.sensor);
     if(giro.binario != giro.anterior){
       giro.pulsos++;
     }
     giro.anterior = giro.binario;
   }
   

// FIM VERIFICACAO POSICAO GIRO DO ESTADO 1 __________________________________________________________________
   
  
 // INICIO VERIFICACAO POSICAO DISTANCIA DO ESTADO 1 -------------------------------------------------------------
    if(distancia.pulsos == distancia.setPoint && (giro.pulsos == giro.setPoint)){
     digitalWrite(distancia.mais,0);
     digitalWrite(distancia.menos,0);
   
   }
    else if(distancia.pulsos != distancia.setPoint && (giro.pulsos == giro.setPoint)){
     digitalWrite(distancia.mais,1);
     digitalWrite(distancia.menos,0);
     
     distancia.binario = digitalRead(distancia.sensor);
      if(distancia.binario != distancia.anterior){
       distancia.pulsos++;
     } 
     distancia.anterior = distancia.binario;
      }

   
  // FIM VERIFICACAO POSICAO DISTANCIA DO ESTADO 1 __________________________________________________________________
 
 // INICIO VERIFICACAO POSICAO ALTURA DO ESTADO 1 -------------------------------------------------------------
    if(altura.pulsos == altura.setPoint && (distancia.pulsos == distancia.setPoint)){
     digitalWrite(altura.mais,0);
     digitalWrite(altura.menos,0);  
    }
    else if(altura.pulsos != altura.setPoint && (distancia.pulsos == distancia.setPoint)){
     digitalWrite(altura.mais,0);
     digitalWrite(altura.menos,1);
     
     altura.binario = digitalRead(altura.sensor);
     if(altura.binario != altura.anterior){
       altura.pulsos++;
     }
      altura.anterior = altura.binario;
    }    
 // FIM VERIFICACAO POSICAO ALTURA DO ESTADO 1 __________________________________________________________________
 
 // INICIO VERIFICACAO POSICAO GARRA DO ESTADO 1 -------------------------------------------------------------
    if(garra.pulsos == garra.setPoint && (altura.pulsos == altura.setPoint)){ // evento que leva ao estado 2 
     digitalWrite(garra.mais,0);
     digitalWrite(garra.menos,0); 
     delay(400);
     inicializar = {0,0,1,0,0,0,0,0,0};
     altura.pulsos=0;
     giro.pulsos=0;
     garra.pulsos=0;
     distancia.pulsos=0;
     giroeg.pulsos=0;
     distancia.binario=0;
     distancia.anterior=0;
     altura.binario=0;
     altura.anterior=0;
     giro.binario=0;
     giro.anterior=0;
     garra.binario=0;
     garra.anterior=0;
     
     
     giroeg.pulsos=0;
    }
    else if(garra.pulsos != garra.setPoint && (altura.pulsos == altura.setPoint)){
     digitalWrite(garra.mais,1);
     digitalWrite(garra.menos,0);
     
     garra.binario = digitalRead(garra.sensor);
     if(garra.binario != garra.anterior){
       garra.pulsos++;
     }
     garra.anterior = garra.binario;
    }
  
     
 // FIM VERIFICACAO POSICAO GARRA DO ESTADO 1 __________________________________________________________________

 }
 
 
 
 
 
 
 
    // ********************************************************************* ESTADO 2: POSICIONAR O MANIPULADOR EM RELAÇÃO À ESTEIRA
    
    
    
    while((inicializar.estado2 == 1)){ 
      
      Serial.print(giro.pulsos);
      Serial.print(" ");
      Serial.print(distancia.pulsos);
      Serial.print(" ");
      Serial.print(altura.pulsos);
      Serial.print(" ");  
      Serial.println(garra.pulsos);  
      
      digitalWrite(ets1.menos,0);
      digitalWrite(ets1.mais,0);
      digitalWrite(ets2.mais,0);
      
      
    altura.setPoint = 75;
    giro.setPoint = 64;
    //Serial.println("ESTADO 3, COLOCANDO PECA NA ESTEIRA");
        
    // INICIO PRIMEIRA VERIFICACAO POSICAO ALTURA DO ESTADO 2 -------------------------------------------------------------  
      if(altura.pulsos == altura.setPoint || auxiliar == 1){
        digitalWrite(altura.mais, 0);
        digitalWrite(altura.menos, 0);
      }
      else if(altura.pulsos != altura.setPoint && auxiliar != 2){
                
         digitalWrite(altura.mais, 1);
         digitalWrite(altura.menos, 0);
         
         altura.binario = digitalRead(altura.sensor);
         if(altura.binario != altura.anterior){
           altura.pulsos++;
         }
         altura.anterior = altura.binario;
         
      }
    // FIM PRIMEIRA VERIFICACAO POSICAO ALTURA DO ESTADO 2 ________________________________________________________________ 
    
    // INICIO VERIFICACAO POSICAO GIRO DO ESTADO 2 -------------------------------------------------------------
      
      if(digitalRead(giro.fim) == 0 && altura.pulsos == altura.setPoint ){
          
          while(digitalRead(giro.fim) != 1){//gira sentido horario até fdc
          digitalWrite (giro.menos, 1);
          digitalWrite (giro.mais, 0); 
           }
          digitalWrite (giro.mais, 0);
          digitalWrite (giro.menos, 0);
          inicializar = {0,0,0,1,0,0,0,0,0};
          altura.pulsos=0;
          giro.pulsos=0;
          garra.pulsos=0;
          distancia.pulsos=0;
          giroeg.pulsos=0;
          distancia.binario=0;
          distancia.anterior=0;
         altura.binario=0;
         altura.anterior=0;
         giro.binario=0;
         giro.anterior=0;
         garra.binario=0;
         garra.anterior=0;
          
        }        
      
      //FIM VERIFICACAO POSICAO GIRO DO ESTADO 2 ___________________________________________________________________________
    } 
   
   
   
   
   // ********************************************************************* ESTADO 3: COLOCAR PEÇA NA ESTEIRA  (e posicionar EG, caso necessário)
   
    
        
   while((inicializar.estado3 == 1)){ 
     
      Serial.print(distancia.pulsos);
      Serial.print(" ");
      Serial.print(altura.pulsos);
      Serial.print(" ");  
      Serial.print(garra.pulsos);  
      Serial.print(" ");  
      Serial.println(giroeg.pulsos);  
     
      digitalWrite(ets1.menos,0);
      digitalWrite(ets1.mais,0);
      digitalWrite(ets2.mais,0);
     
    distancia.setPoint= 80;
    giroeg.setPoint=111 ;
    altura.setPoint=61;
    
   // INICIO VERIFICACAO POSICAO DISTÂNCIA DO ESTADO 3 -------------------------------------------------------------
   
    if(distancia.pulsos == distancia.setPoint ){
     digitalWrite(distancia.mais,0);
     digitalWrite(distancia.menos,0);     
   
   }
    else if(distancia.pulsos != distancia.setPoint ){
     digitalWrite(distancia.mais,0);
     digitalWrite(distancia.menos,1);
     
     distancia.binario = digitalRead(distancia.sensor);
      if(distancia.binario != distancia.anterior){
       distancia.pulsos++;
     } 
     distancia.anterior = distancia.binario;
      }
    // FIM SEGUNDA VERIFICACAO POSICAO DISTÂNCIA DO ESTADO 3 ___________________________________________________________________
    
   
    // INICIO VERIFICACAO POSICAO EG DO ESTADO 3 -------------------------------------------------------------
    if(digitalRead(giroeg.fim) == 1 || auxiliar == 1){
      
      auxiliar=1; 
      if(giroeg.pulsos == giroeg.setPoint ){
       digitalWrite(giroeg.mais,0);
       digitalWrite(giroeg.menos,0);
       
     }
      else if(giroeg.pulsos != giroeg.setPoint){
       digitalWrite(giroeg.mais,0);
       digitalWrite(giroeg.menos,1);
     
       giroeg.binario = digitalRead(giroeg.sensor);
        if(giroeg.binario != giroeg.anterior){
         giroeg.pulsos++;
       } 
       giroeg.anterior = giroeg.binario;
        }
    } 
    
  // FIM VERIFICACAO POSICAO E DO ESTADO 3 ___________________________________________________________________      
               
    // INICIO VERIFICACAO POSICAO ALTURA DO ESTADO 3 ------------------------------------------------------------- 
      if(altura.pulsos == altura.setPoint ){
        digitalWrite(altura.mais, 0);
        digitalWrite(altura.menos, 0); 
      }
      else if(altura.pulsos != altura.setPoint ){
        digitalWrite(altura.mais, 0);
        digitalWrite(altura.menos, 1);
        
       altura.binario = digitalRead(altura.sensor);
       if(altura.binario != altura.anterior){
         altura.pulsos++;
       }
        altura.anterior = altura.binario;   
      }    
    // FIM VERIFICACAO POSICAO ALTURA DO ESTADO 3 ___________________________________________________________________
      
   // INICIO VERIFICACAO POSICAO GARRA DO ESTADO 3 ------------------------------------------------------------- 
      if(digitalRead(garra.fim) == 1 && altura.pulsos == altura.setPoint){ //evento que leva ao estado 3
        digitalWrite(garra.mais, 0);
        digitalWrite(garra.menos, 0);
        inicializar = {0,0,0,0,1,0,0,0,0};
        altura.pulsos=0;
        giro.pulsos=0;
        garra.pulsos=0;
        distancia.pulsos=0;
        giroeg.pulsos=0;
        distancia.binario=0;
       distancia.anterior=0;
       altura.binario=0;
       altura.anterior=0;
       giro.binario=0;
       giro.anterior=0;
       garra.binario=0;
       garra.anterior=0;
      }
      else if(digitalRead(garra.fim) != 1 && altura.pulsos == altura.setPoint ){
        digitalWrite(garra.mais, 0);
        digitalWrite(garra.menos, 1);
      }
   // FIM VERIFICACAO POSICAO GARRA DO ESTADO 2 ____________________________________________________________________________  
        
         
   }
   
   
   
 
 
 
 
 
   // ****************************************************************ESTADO 4: CLASSSIFICAÇÃO DO TAMANHO DO BLOCO
   
   
 
   while(inicializar.estado4 ==1){
   
     
      Serial.print(giro.pulsos);
      Serial.print(" ");
      Serial.print(distancia.pulsos);
      Serial.print(" ");
      Serial.print(altura.pulsos);
      Serial.print(" ");  
      Serial.println(garra.pulsos);  
 
       digitalWrite(ete.mais,0);
      digitalWrite(ets1.menos,0);
      digitalWrite(ets1.mais,0);
      digitalWrite(ets2.mais,0);
      

       if(!digitalRead(ete.fte_entrada)){ //Sensor inferior da ETE (esteira de entrada)
         
         delay(150);
       
         if (!digitalRead(ete.fte_saida)){ // Sensor superior da ETE
           grande=1;
           inicializar = {0,0,0,0,0,1,0,0,0};
           altura.pulsos=0;
           giro.pulsos=0;
           garra.pulsos=0;
           distancia.pulsos=0;
           giroeg.pulsos=0;
           distancia.binario=0;
           distancia.anterior=0;
           altura.binario=0;
           altura.anterior=0;
           giro.binario=0;
           giro.anterior=0;
           garra.binario=0;
     garra.anterior=0;
           //Serial.println("grande");
           auxiliar ==2;
         }
         
         else if(auxiliar !=2){
         
         grande=0;
         inicializar = {0,0,0,0,0,1,0,0,0};
         altura.pulsos=0;
         giro.pulsos=0;
         garra.pulsos=0;
         distancia.pulsos=0;
         giroeg.pulsos=0;
         distancia.binario=0;
         distancia.anterior=0;
         altura.binario=0;
         altura.anterior=0;
         giro.binario=0;
         giro.anterior=0;
         garra.binario=0;
         garra.anterior=0;
         //Serial.println("pequeno");
         
         }
 
      }
     
   }
     

   
    // *******************************************************************ESTADO 5 : RECEBER O BLOCO NA E.R.
  
  while((inicializar.estado5 == 1) ){
    
    Serial.print(giro.pulsos);
      Serial.print(" ");
      Serial.print(distancia.pulsos);
      Serial.print(" ");
      Serial.print(altura.pulsos);
      Serial.print(" ");  
      Serial.println(garra.pulsos);  
  
    
    
    if(!digitalRead(eg.fte_meio)){ // Verifica se a peça ja chegou no meio da EG (esteira rotatoria)
    
      digitalWrite(eg.mais, 0);
      digitalWrite(eg.menos,0);
      digitalWrite(ete.mais,1);
      
      if(digitalRead(giroeg.fim) ==1){
       
       digitalWrite(giroeg.mais,0);
       digitalWrite(giroeg.menos,0);
      
      }
      
      else{
        
       digitalWrite(giroeg.mais,1);
       digitalWrite(giroeg.menos,0);
      
      }
      
      
      if(grande == 1 && digitalRead(giroeg.fim) ==1){ //Se grande, transita para o estado 6
        inicializar = {0,0,0,0,0,0,1,0,0};
      }
      else if (grande == 0 && digitalRead(giroeg.fim) ==1){ //Se pequena, transita para o estado 7
       inicializar = {0,0,0,0,0,0,0,1,0}; 
      }
      altura.pulsos=0;
      giro.pulsos=0;
      garra.pulsos=0;
      distancia.pulsos=0;
      giroeg.pulsos=0;
      distancia.binario=0;
       distancia.anterior=0;
       altura.binario=0;
       altura.anterior=0;
       giro.binario=0;
       giro.anterior=0;
       garra.binario=0;
       garra.anterior=0;
      digitalWrite(ets1.menos,0);
      digitalWrite(ets1.mais,0);
      digitalWrite(ets2.mais,0);
    }
    
    else{
      
      digitalWrite(eg.mais, 1);
      digitalWrite(eg.menos,0);
      digitalWrite(ete.mais,0);
    
    }
  
  } 
  
  
  
 
   // *******************************************************************ESTADO 6 : DIRECIONA BLOCO GRANDE
   while((inicializar.estado6 == 1)){

     delay(200);

      if(grande == 1){
      
        if(!digitalRead(ets1.fte_entrada)){
        
           do{           
              digitalWrite(eg.mais, 0);
              digitalWrite(eg.menos,1); 
           }
           while(!digitalRead(ets1.fte_entrada)); 
             
             esteira_off=1;
             digitalWrite(eg.mais, 0);
             digitalWrite(eg.menos,0);                   
        
        }
       else if(digitalRead(ets1.fte_entrada) && esteira_off != 1){
       
         digitalWrite(eg.mais, 0);
         digitalWrite(eg.menos,1);
       }
        
        if(ets11 != 0){
          
           digitalWrite(ets1.menos,0);
           digitalWrite(ets1.mais,1);
           if(esteira_off==1) pulsos_ets1++;         
      }  
      
      if(pulsos_ets1 == 19 && digitalRead(ets1.fte_saida)){
      
        ets11 = 0;
        digitalWrite(ets1.menos,1);          
        digitalWrite(ets1.mais,0);
        
      
      } 
           
           
         
         else if(!digitalRead(ets1.fte_saida)) {
          
            do {
              digitalWrite(eg.mais, 0);
             digitalWrite(eg.menos,0);
             digitalWrite(ets1.menos,1);
             digitalWrite(ets1.mais,0);
            }
            while(!digitalRead(ets1.fte_saida));
          
           digitalWrite(eg.mais, 0);
           digitalWrite(eg.menos,0); 
           digitalWrite(ets1.menos,0);
           digitalWrite(ets1.mais,0);
           inicializar = {0,0,0,0,0,0,0,0,1};
           altura.pulsos=0;
           giro.pulsos=0;
           garra.pulsos=0;
           distancia.pulsos=0;
           giroeg.pulsos=0; 
           distancia.binario=0;
           distancia.anterior=0;
           altura.binario=0;
           altura.anterior=0;
           giro.binario=0;
           giro.anterior=0;
           garra.binario=0;
           garra.anterior=0;  
         }
         
                 
      }
      
    
    }
     
     
  
  
  
  
   // *******************************************************************ESTADO 7 : DIRECIONA BLOCO PEQUENO
 
   while((inicializar.estado7 == 1)){
      
     delay(200);
            
      if(grande == 0){
      
      Serial.println(digitalRead(ets2.fte_entrada));
      
         if(!digitalRead(ets2.fte_entrada)){
        
           do{
           
              digitalWrite(eg.mais, 1);
              digitalWrite(eg.menos,0); 
           }
           while(!digitalRead(ets2.fte_entrada));
             
             esteira_off=2;
             digitalWrite(eg.mais, 0);
             digitalWrite(eg.menos,0);                   
        
        }
       else if(digitalRead(ets2.fte_entrada) && esteira_off != 2){
       
         digitalWrite(eg.mais, 1);
         digitalWrite(eg.menos,0);
       }
        
         if(digitalRead(ets2.fte_saida) && auxiliar != 100){
          
           digitalWrite(ets2.mais, 1);

         }
         else if(!digitalRead(ets2.fte_saida)) {
          
           do{
            
              digitalWrite(ets2.mais, 1);
              auxiliar=24;
           } while(!digitalRead(ets2.fte_saida));
            
           if(auxiliar == 24){
           
             auxiliar= 100;
           digitalWrite(eg.mais, 0);
           digitalWrite(eg.menos,0);
           digitalWrite(ets2.mais,0);
           inicializar = {0,0,0,0,0,0,0,0,1};
           altura.pulsos=0;
           giro.pulsos=0;
           garra.pulsos=0;
           distancia.pulsos=0;
           giroeg.pulsos=0;
           distancia.binario=0;
           distancia.anterior=0;
           altura.binario=0;
           altura.anterior=0;
           giro.binario=0;
           giro.anterior=0;
           garra.binario=0;
           garra.anterior=0;}
           else{
           inicializar = {0,0,0,0,0,0,0,1,0};
           }
         }
         
       }
   }    
           
 
 
 
 
 
   // *******************************************************************ESTADO 8 : VOLTAR PARA POSIÇÃO INICIAL
   
  while((inicializar.estado8 == 1)){ 
    
     digitalWrite(eg.mais, 0);
     digitalWrite(eg.menos,0);
     digitalWrite(ets1.mais,0);
  // Serial.println("ESTADO 4, VOLTANDO PARA POSICAO INICIAL");
     if(digitalRead(altura.fim) == 1){
       digitalWrite(altura.mais, 0);
       digitalWrite(altura.menos, 0);
     }
     else if(digitalRead(altura.fim) == 0){
       digitalWrite(altura.mais, 1);
       digitalWrite(altura.menos, 0);
     }
     
     if(digitalRead(distancia.fim) == 1  && digitalRead(altura.fim) == 1){ // evento que leva ao estado 4
       
       //ATINGIU O FIM DO PROCESSO, DESLIGAR TODOS MOTORES:
       digitalWrite(distancia.mais, 0);
       digitalWrite(distancia.menos, 0);
       digitalWrite (altura.mais, 0);
       digitalWrite (altura.menos, 0); 
       digitalWrite (giro.mais, 0); 
       digitalWrite (giro.menos, 0);
       digitalWrite (garra.mais, 0); 
       digitalWrite (garra.menos, 0); 
       //________________________________________________________
       inicializar = {0,0,0,0,0,0,0,0,0};
       Evento_final=1;
       altura.pulsos=0;
       giro.pulsos=0;
       garra.pulsos=0;
       distancia.pulsos=0;
       giroeg.pulsos=0;
       distancia.binario=0;
       distancia.anterior=0;
       altura.binario=0;
       altura.anterior=0;
       giro.binario=0;
       giro.anterior=0;
       garra.binario=0;
       garra.anterior=0;
       pulsos_ets1=0;
       ets11 = 1;    

     }
     else if(digitalRead(distancia.fim) == 0 && digitalRead(altura.fim) == 1){
       digitalWrite(distancia.mais, 0);
       digitalWrite(distancia.menos, 1);
     }
    
   }
  }

}
