import os
f=open("log.txt","r")

check=0
while True:
    strr=f.readline()
    if cmp(strr,"")==0:
        break
    strrs=strr.split(" ")

    if cmp(strrs[0],"node")==0:
        count=0
        allpack=strrs[5]
        #print "ok"
        check=1
    if check==1:
        
        if cmp(strrs[0],"At")==0:
            delay=strrs[2]
            count=count+1
        if cmp(strrs[0],"end\r\n")==0:
            persent=(float(allpack)-float(count))/float(allpack)
            delay=float(delay[0:delay.index("s")])-2
            
            #print str(delay)
            print str(persent*100)
            #print str(delay)+"\t"+"package loss:"+str(persent)+"%"
            #print str(persent)
            #print str(count)
            
            check=0
