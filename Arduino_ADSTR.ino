const int sensorPin= A0;
int opcio=0;
String data;
String data1;
int temps;
int tempsd;
int tempsu;
int mostres;
int tempsentre;
float mytemp[10];
int i=0;
int k=0;
float mitjana=0;
int comptador=0;
int j=1;
int demana=0;
int parat=1;
float mitjana2=0;
int pin;
int estat;
int sortidau;
int sortidad;
int sortida;
int entradau;
int entradad;
int entrada;
int valorentrada;
String msgE="AE0EZ";
String msgS="ASEZ";
String msgM="AMEZ";
String msgC="ACE0000Z";
int mitjana3=0;
int error1=1;
float tempC;

 
//Void per a la funció parada
void parada() {
  
   //comprovació si l'arduino està connectat
   if (Serial.available())
   {

     principi:
     
     //Recollida del valor introduit
     data = Serial.readStringUntil('\n');
     
      //comprovació d'una segona A avans de la Z
     if(data[0]=='A'){

          
          while((data[error1]!='Z') && (error1 <= data.length())){

              if(data[error1]=='A'){

                data[0]='K';
                goto principi;                 
              }
              else{
                error1++;
              }
          }
        }
        error1=1;    
     //comprovació de la funció Marxa
      if((data[0]=='A')&&(data[1]=='M')&&(data[2]=='1')&&(data[6]=='Z')){
        
        //conversió del número de string (ASCII) a enter
        tempsd=data[3]-48;
        tempsu=data[4]-48;
        
        //conversió a número enter de 2 dígits
        temps=tempsd*10+tempsu; 
        mostres=data[5]-48;
        
        //temps entre mostres en segons
        tempsentre=temps*1000;

        //Comprovació del si el temps supera els 20 segons o les mostres són superiors a 9
        if((temps <= 60)&&(temps > 0)&&(mostres <= 9)&&(mostres > 0)){
          
         //Igualació per evitar mostrar continuament el missatge introduït
          data1=data;
          
          //Canvi de número a caràcter ASCII, sumant 48 (valor 0 en ASCII)
          msgM[2]=0+'0';
          
          //Missatga de codi correcte
          
          Serial.println(msgM);
                    
          //es manté en situació marxa
          opcio=1;
        }

         else{
           
        //en cas contrari, s'envia un 2, error de paràmetres
          msgM[2]=2+'0';
          Serial.println(msgM);
         }
      }

      if((data[0]=='A')&&(data[1]=='M')&&(data[2]=='0')&&(data[6]=='Z')){

        msgM[2]=2+'0';
        Serial.println(msgM);
      }
      
        //Comprovació d'error de protocol
      if((data[0]=='A') && (data[1]=='M') && (data[6]!='Z')){
     
          msgM[2]=1+'0';
          Serial.println(msgM);
     }
       //Comprovació ordre mitjana
      if((data[0]=='A') && (data[1]=='C') && (data[2]=='Z')){
       
       //mitjana anterior si està guardada
        if(mitjana2 != 0){
            
            //Variable per deixar la mitjana sense decimals
            mitjana3=mitjana2*100;
            
            //Adquisició dels valor de mitjana 1 a 1, per poder enviar posteriorment
            msgC[2]=2+'0';
            msgC[3]=((mitjana3-(mitjana3%1000))/1000)+'0';
            msgC[4]=(((mitjana3%1000)-(mitjana3%100))/100)+'0';
            msgC[5]=(((mitjana3%100)-(mitjana3%10))/10)+'0';
            msgC[6]=((mitjana3%10))+'0';
            
            //Mostra per pantalla el valor de la mitjana           
            Serial.println(msgC); 
        }
        else{
          
          //En cas contrari, error de paràmetres
          msgC[2]=2+'0';
          msgC[3]='0';
          msgC[4]='0';
          msgC[5]='0';
          msgC[6]='0';
        
          Serial.println(msgC);
          
        }
       
      }
      
      //Comprovació de conversió analógic digital
      if((data[0]=='A') && (data[1]=='C') && (data[2]!='Z')){
          //Mostrar per pantalla error de protocol
          msgC[2]=1+'0';
          msgC[3]='0';
          msgC[4]='0';
          msgC[5]='0';
          msgC[6]='0';
          Serial.println(msgC);
      }
      
      //Comprovació comanda Sortida digital
      if((data[0]=='A') && (data[1]=='S') && (data[5]=='Z')){
        
       //Conversió en números enters
        sortidad=data[2]-48;
        sortidau=data[3]-48;
        sortida=sortidad*10+sortidau;
        
        //comprovació del nº de pin de sortida dins del rang de l'arduino i que s'activi o es desactivi (1 o 0)
          if((sortida <= 13) && ((data[4]=='1')||(data[4]=='0'))){
            
              //Mode pin activat
              if(data[4]=='1'){
                
              //Activació sortida i declaració de sortida com la variable "sortida"
                pinMode(sortida,OUTPUT);
                digitalWrite(sortida,HIGH);
                
              //Mostra per pantalla codi correcte
                msgS[2]=0+'0';
                
                Serial.println(msgS);
              }
              //Mode pin desactivat
              if(data[4]=='0'){
                
              //Desactivació sortida i declaració de sortida com la variable "sortida"
                pinMode(sortida,OUTPUT);
                digitalWrite(sortida,LOW);
              //Mostrar per pantalla codi correcte
                msgS[2]=0+'0';
                
                Serial.println(msgS);
              }           
            }
            else{
            //Mostrar error de paràmetres
              msgS[2]=2+'0';
              Serial.println(msgS);
            }
            
        }
        //Comprovació d'error de protocol
      if((data[0]=='A') && (data[1]=='S') && (data[5]!='Z')){

          
          msgS[2]=1+'0';
          Serial.println(msgS);
      }
      
      //Comprovació Codi entrades, mateix funcionament que el codi de sortides
      if((data[0]=='A') && (data[1]=='E') && (data[4]=='Z')){
        
        entradad=data[2]-48;
        entradau=data[3]-48;
        entrada=entradad*10+entradau;

        if(entrada <= 13){
          
        //Mode pullup per evitar rebots i assegurar un 1 quan no s'utilitza aquesta entrada
        pinMode(entrada,INPUT_PULLUP);
        
        //Lectura del valor de l'entrada
        valorentrada= digitalRead(entrada);
        //Mostra del valor de lectura        
        msgE[3]=valorentrada+'0';
        Serial.println(msgE);
        }
        
        //Comprovació de la lectura d'entrades que no sigui major al rang de l'arduino
        if(entrada > 13){

          msgE[2]=2+'0';
          msgE[3]=0+'0';
          Serial.println(msgE);
        }
      }
      
      //Comprovació error de protocol d'entrades
      if((data[0]=='A') && (data[1]=='E') && (data[4]!='Z')){

          msgE[2]=1+'0';
          msgE[3]=0+'0';
          Serial.println(msgE);
      }     
   }
}

      //Funció marxa
void marxa() {
    
  if (Serial.available())
   {
      //Valor introduit per consola    
      data = Serial.readStringUntil('\n');

      principi2:
      //Comprovació de si el valor de data1 es igual a data per no mostrar continuament el valor
      if (data.equals(data1)) {
      }
            
      else{
         //comprovació d'una segona A abans de Z
        if(data[0]=='A'){

          
          while((data[error1]!='Z') && (error1 <= data.length())){

              if(data[error1]=='A'){

                
                data[0]='X';
                goto principi2;   
              }
              else{
              error1++;
              }  
          }
        }
        error1=1;
        //comprovació comanda Mitjana  
        if((data[0]=='A') && (data[1]=='C') && (data[2]=='Z')){

          
          //Cridada a la funció funciomitjana
          funciomitjana();
                  
        }
        //Error de protocol
        if((data[0]=='A') && (data[1]=='C') && (data[2]!='Z')){
          
          msgC[2]=1+'0';          
          Serial.println(msgC);
      }
        //Comprovació codi sortida
        if((data[0]=='A') && (data[1]=='S') && (data[5]=='Z')){
        
          sortidad=data[2]-48;
          sortidau=data[3]-48;
          sortida=sortidad*10+sortidau;
        
        //Comprovació Rang sortides arduino
        if((sortida <= 13) && ((data[4]=='1')||(data[4]=='0'))){
        
            if(data[4]=='1'){
    
              pinMode(sortida,OUTPUT);
              digitalWrite(sortida,HIGH);
              
              msgS[2]=0+'0';
              Serial.println(msgS);
            }
            if(data[4]=='0'){
    
              pinMode(sortida,OUTPUT);
              digitalWrite(sortida,LOW);
    
              msgS[2]=0+'0';
              Serial.println(msgS);
            }           
          }
          else{
            //Error de paràmetres
            msgS[2]=2+'0';
            Serial.println(msgS);
          }
          
      }
      //Error de protocol
      if((data[0]=='A') && (data[1]=='S') && (data[5]!='Z')){

          
          msgS[2]=1+'0';
          Serial.println(msgS);
      }
    
      //Comprovació d'entrades
      if((data[0]=='A') && (data[1]=='E') && (data[4]=='Z')){
              
        entradad=data[2]-48;
        entradau=data[3]-48;
        entrada=entradad*10+entradau;

       //Comprovació rang d'entrades
        if(entrada <= 13){
          
        pinMode(entrada,INPUT_PULLUP);
        
        valorentrada= digitalRead(entrada);

        msgE[3]=valorentrada+'0';
        Serial.println(msgE);
        }
        
        //Error de paràmetres
        if(entrada > 13){

          msgE[2]=2+'0';
          msgE[3]=0+'0';
          Serial.println(msgE);
        }
      }
      //Error de protocol
      if((data[0]=='A') && (data[1]=='E') && (data[4]!='Z')){

          msgE[2]=1+'0';
          msgE[3]=0+'0';
          Serial.println(msgE);
      }
      
      //Comprovació codi marxa i parada de l'adquisició
     if((data[0] == 'A')&&(data[1]=='M')&&(data[2]=='0')&&(data[6]=='Z')){
        
       //Variable per al case parada
          parat=1;
          opcio=0;
          msgM[2]=0+'0';
          Serial.println(msgM);
        }
      //Error de paràmetres
     if((data[0]=='A') && (data[1]=='M') && (data[6]!='Z')){

          msgM[2]=1+'0';
          Serial.println(msgM);

     }

        else{
          //Nova adquisició de marxa
           if((data[0]=='A')&&(data[1]=='M')&&(data[2]=='1')&&(data[6]=='Z')){

              tempsd=data[3]-48;
              tempsu=data[4]-48;
              temps=tempsd*10+tempsu;
              mostres=data[5]-48;
              tempsentre=temps*1000;
      
              if((temps <= 20)&&(temps > 0)&&(mostres <= 9)&&(mostres > 0)){
         
                  data1=data;
                  msgM[2]=0+'0';
                  Serial.println(msgM);
                  opcio=1;
                }

              else{
                //Error de paràmetres
                msgM[2]=2+'0';
                Serial.println(msgM);
              }  
           }  
        } 
      }  
   }
}
//Void de la funció funciomitjana
void funciomitjana(){
  
  //i predefinida com a valor 0
    if(i == 0){
      //variable per a l'array circular
      k=8;
    }
    else{
      k=i-1;
    }
    //Bucle per agafar nomes les mostres establertes per fer la mitjana
    while(j <= (mostres)){
      
      mitjana=mitjana+mytemp[k];

      
      if(k != 0){
        k--;
      }
      else{
        k=8;
      }
      j++;
      
    }
    //Es realitzen els calculs per fer la mitjana
    mitjana=mitjana/mostres;
    mitjana2=mitjana;
    
    j=1;

            //Es trasllada la mitjana numero per numero a un String, per treure el missatge per panatalla
      mitjana3=mitjana2*100;

      msgC[2]=0+'0';
      msgC[3]=((mitjana3-(mitjana3%1000))/1000)+'0';
      msgC[4]=(((mitjana3%1000)-(mitjana3%100))/100)+'0';
      msgC[5]=(((mitjana3%100)-(mitjana3%10))/10)+'0';
      msgC[6]=((mitjana3%10))+'0';

      Serial.println(msgC);
      
      mitjana=0;      
}
//Void de la funció adquisició
void adquisicio() {

      //Es comprova que s'ha d'activar l'adquisició
      if(opcio == 1){
        
        comptador++;    
      //Assignem cada nova adquisició a una posició de l'array de temperatures
      if(comptador == temps){
        
          int value = analogRead(sensorPin);
          float millivolts = (value / 1023.0) * 5000;
          float celsius = millivolts / 10; 
          mytemp[i]=celsius;
          i++;
          comptador=0;

          if(i >= 9){

            i=0;          
          }
      }
    }
}
//Void de la funció setup
void setup()
{
  Serial.begin(9600);
  noInterrupts(); // disable all interrupts
   TCCR1A = 0;
   TCCR1B = 0;
   TCNT1 = 0;
   OCR1A = 62500; // compare match register 16MHz/256/2Hz
   TCCR1B |= (1 << WGM12); // CTC mode
   TCCR1B |= (1 << CS12); // 256 prescaler
   TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
   interrupts(); // enable all interrupts
}

ISR(TIMER1_COMPA_vect) // timer compare interrupt service

{
  //Cridem la funció adquisició
  adquisicio();

}
//Void de la funció loop
void loop()
{
  switch (opcio) {
    //Mode PARADA, es crida la funció parada, comentada anteriorment
    case 0:
      
      if(parat == 1){
        
        parat=0;
      }
      parada();
      break;
    //Mode MARXA, es crida la funció marxa, comentada anteriorment 
    case 1:
    
      marxa();
      break;   
  }
}
//FI del codi
