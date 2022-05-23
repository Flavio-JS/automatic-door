#define echoPin 9		//pino que recebe o pulso echo
#define trigPin 10		//pino que envia o pulso para gerar o echo

unsigned int duracao = 0;
unsigned int distancia = 0;
int segurancaAbre = 0;
int segurancaFecha = 15;
int trava = 0;

void setup(){
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  digitalWrite(trigPin, HIGH); //eleva o sinal do trigger
  delayMicroseconds(10); // por 10 ms
  digitalWrite(trigPin, LOW);//diminui o sinal do trigger

  duracao = pulseIn(echoPin, HIGH); //armazena o valor em ms que foi lido

  distancia = duracao*0.017; 	/*((Velocidade do som no ar m/s)*(100 para transformar em cm)
  								((Vs no ar)*(100)/1000000 pois a duração é em microsegundos)
                                ((Vs*100)/10^6)/2(pois o som vai até o objeto e volta)
                                ((340*100)/(2*10^6))=0,017  */

  if(distancia<=10){ //distancia minima para abrir a porta
    analogWrite(A3, 0); //sinal de fechar a porta desligado
    while(segurancaAbre<15){//segurancaAbre começa em 0 (14 loops)
      analogWrite(A2, 155) ;//sinal de abrir a porta ligado
      segurancaAbre++;
      delay(500);//delay de 0,5s -- leva 7s para abrir a porta
    }
    segurancaFecha=15;/*segurancaFechada=15 para fechar a porta
    				   no mesmo tempo que abriu*/
    trava=1;//trava=0 não permite fechar a porta --- trava=1 permite
    analogWrite(A2, 0) ;//sinal de abrir a porta desligado
  }
  digitalWrite(trigPin, HIGH);//eleva o sinal do trigger
  delayMicroseconds(10);// por 10 ms
  digitalWrite(trigPin, LOW);//diminui o sinal do trigger
  duracao = pulseIn(echoPin, HIGH);
  distancia = duracao*0.017;
  if(distancia>10 && segurancaAbre==15 && trava == 1){/*porta fecha se não
  										houver nenhum objeto a 10cm do sensor
                                        e se a porta foi aberta anteriormente
                                        e se a trava permitir*/
    while(segurancaFecha>0){//14 loops
      analogWrite(A3, 155);//sinal de fechar a porta ligado
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duracao = pulseIn(echoPin, HIGH);
      distancia = duracao*0.017;
      segurancaFecha--;
      delay(500); // LEMBRAR colocar dps do delay
      if(distancia<=10){//freio para caso a pessoa não saiu da frente da porta
        break;//sai do loop que fecha a porta
      }
    }
    segurancaAbre=segurancaFecha;
    trava=0;//trava=0 não permite fechar a porta --- trava=1 permite
    analogWrite(A3, 0);//sinal de fechar a porta desligado
  }
}
