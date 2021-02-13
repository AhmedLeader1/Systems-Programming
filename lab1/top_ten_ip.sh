######################################################################################################################
#         Student Name:Ahmed Ahmed                                                                                   #
#         Student #: 0632851                                                                                         #
#                                                                                                                    #
#         Purpose: The main objective of this lab is to create a script that outputs all the IP addresseses which    #
#                  are generating "Authentication failures", and the count(how many times) they appear in the logs.  #
#                  Lastly the code displays the original countires in which the IP addresses were accessed from.     #
#                                                                                                                    #
#                  The Process of determining the IP address starts by "greping" all the lines that contain          #
#                  "Authentication failure"from all the file in "/home/cois/3380 directory. It then sorts            #
#                  numerically, and isolate the unique IP addressesses from the rest.The program then countes the    #
#                  frequecy of thier occurences before it sorts them reverse numberically.                           #
#                                                                                                                    #
#                  Lastly, the program uses only the top ten addresses, thier counts and the countires they are      #
#                  accessed from in descending order.                                                                #
#                                                                                                                    #
#                  One of the real life applications of such a code is to determine the hackers' location, the       #
#                  frequecy they attempt to hack a system, and if the documents they target in thier hacking process.#
#                                                                                                                    #
#                                                                                                                    #
#                                                                                                                    #
#                                                                                                                    #
######################################################################################################################

# This line of code 1st it greps all the lines that contain "authentication failure" from all the files in the directory,
# It then carry out a grep based on regex ip address command, and sorts the IPs numerically
# the code then itentifies only the unique IP addresses from the rest, and counts thier occurence
# Lastly, the code sorts the IPs in reverse numberically

grep -e "authentication failure" /home/COIS/3380/secure* | grep -o '[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9-]\{1,3\}\.[0-9]\{1,3\}' | sort -n | uniq -c | sort -rn | head > top_ten_ip.txt

#This line of code displays the IP addresses with thier occurences 
#The command uses the awk command to keep the right order of the IP addresses
#Lastly, the command uses a while loop in order to send the IP addresses to ipinfo.io

grep -e "authentication failur" /home/COIS/3380/secure* | grep -o '[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9-]\{1,3\}\.[0-9]\{1,3\}' | sort -n | uniq -c | sort -rn | 
head | awk '{print $2}' | while read i; do wget -O /home/ahmedahmed/lab1/ipaddresses/$i http://ipinfo.io/$i ; done



#This code uses the wget command to send the IP addresses to the ipinfo.io and obtains the geographical location of the first two IP addresses from the ipinfo.io

wget http://ipinfo.io/58.218.198.150
wget http://ipinfo.io/116.31.116.26


