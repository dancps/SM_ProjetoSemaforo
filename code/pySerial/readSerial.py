import serial as ser
import os
import time

linesPerFiles=100
debug = True
print("Iniciando comunicação serial.")
ESP32 = ser.Serial("COM3",115200)
indexFile=0
while True:
    try:
        resultsDir="./results/"
        fileName = "log_serial_"+str(indexFile)+".csv"
        finalPath = resultsDir+fileName
        print("Abrindo o arquivo "+finalPath)
        log_file = open(finalPath,"w")

        count_lines=0
        line=""

        start = time.time()
        while(count_lines<linesPerFiles):
            line = ESP32.readline()
            if(line[0]!="b"): line = ESP32.readline() # Caso a linha seja lida a partir de qualquer ponto que não o inicial ela é ignorada
                
            if(line.decode('utf-8')[0]=="#"): # Testa se a linha é de log
                if(("#l#" in line.decode('utf-8')) and ("#e#" in line.decode('utf-8'))): # Testa se a linha começa e termina
                    line = line.decode('utf-8') # Encoda a linha
                    line = line.replace("#l#","") # Remove a marcação inicial
                    line = line.replace("#e#","") # Remove a marcação final
                    line = "Time;"+str(time.strftime('%X'))+";Date;"+str(time.strftime('%x'))+";"+ line
                    log_file.write(line) # Grava a linha no arquivo
                    count_lines= count_lines+1
                    if(debug and count_lines%100==0): print(count_lines)
        end = time.time()
        print("Arquivo:"+fileName+"\n    +Foram salvas "+str(linesPerFiles)+" linhas  em "+str(end-start)+"s em um arquivo de "+str(os.path.getsize(finalPath)/1000)+"kB.")
        print("    +Velocidade: "+str((os.path.getsize(finalPath)/1000)/(end-start))+" kB/s")
        print("Fechando o arquivo "+finalPath+"\n")
        log_file.close()
        indexFile = indexFile+1

    except KeyboardInterrupt:
        print("Encerrando comunicação serial.")
        ESP32.close()
        print("Fechando arquivos.")
        log_file.close()
        print("Fechando o programa.")
        exit()
