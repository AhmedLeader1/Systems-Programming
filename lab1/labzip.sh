#===================================================================================================#
# The writer's information                                                                          #
#                                                                                                   #
#  Author: Ahmed Ahmed    Student #: 0632851                                                        #
#  Date: Jan 16th, 2020                                                                             #
#  Purpose of this document: the main purpose of this script is to create a zip file                #
#                            for our labs in order ofr easy submission                              #
#===================================================================================================#

# Variable inputs for the labs

FirstName=ahmed
LastName=ahmed

#This command creates a zip file for the labs
zip -r ${HOME}/${LastName}_${FirstName}_$1.zip ${HOME}/$1/*


