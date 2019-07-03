param(
        [Parameter(Mandatory=$true)][string]$VmName, 
        [Parameter(Mandatory=$false)][string]$UserToken = "openlabbuildmaster:11d3463e22ab89adbb77750e1223d1990d",
        [Parameter(Mandatory=$false)][string]$ScriptsSourceDir = "C:\_GIT\JenkinsScripts\src\agilent\sid\cicd\scripts\ps\",
        [Parameter(Mandatory=$false)][string]$ScriptsDestDir = "C:\Scripts\",
        [Parameter(Mandatory=$false)][string]$JenkinsUrl = "http://scs-jenkins-4.scs.agilent.com:8080"
)

Write-Host "VmName ............ $VmName"
Write-Host "UserToken ......... $UserToken"
Write-Host "ScriptsSourceDir .. $ScriptsSourceDir"
Write-Host "ScriptsDestDir .... $ScriptsDestDir"
Write-Host "JenkinsUrl ........ $JenkinsUrl"


$ErrorActionPreference = 'Stop'

# Set Trusted Hosts on the Computer that is running this script
Set-Item WSMan:\localhost\Client\TrustedHosts -Value $vmName -Force

$userAdmin = "admin"
$adminPwd = "3000hanover" | ConvertTo-SecureString -asPlainText -Force
$creds =New-Object System.Management.Automation.PSCredential($userAdmin, $adminPwd)

# Create a Session to the Remote VM/Computer
$psSession = New-PSSession -ComputerName $vmName -Credential $creds

# Copy all of the scripts over to the Remote VM/Computer
Copy-Item -ToSession $psSession -Path $ScriptsSourceDir  -Destination $ScriptsDestDir -Recurse -ErrorAction Stop

# Use the Session to invoke a series of operations on the Remote VM/Computer
$nodeSecret = Invoke-Command -Session $psSession -ScriptBlock {

    # Stop Execution on any failure
    $ErrorActionPreference = 'Stop'

    # Set the execution policy on the Remote VM/Computer so that we can execute our scripts
    Set-ExecutionPolicy -ExecutionPolicy Unrestricted -Scope CurrentUser | Out-Null
    
    # Change the current directory to the Script directory
    cd "$Using:ScriptsDestDir" | Out-Null
    
    # On the Remote VM/Computer run the PS script that pulls down the Jenkins Agent.jar and Jenkins-cli.jar files
    Invoke-Expression ".\Install_Jenkins_CLI_and_Agent.ps1 -Jenkins_Server_Url $Using:JenkinsUrl"

    # On the Remote VM/Computer run the PS script that creates the Node in Jenkins
    Invoke-Expression ".\CreateJenkinsNode.ps1 -jenkinsUrl $Using:JenkinsUrl -jenkinsNode $Using:VmName"

    # On the Remote VM/Computer run the PS script that gets the Node Secret from Jenkins
    $nodeSecret = Invoke-Expression ".\GetNodeSecret.ps1 -VmName $Using:VmName -UserToken $Using:UserToken -jenkinsUrl $Using:JenkinsUrl"
    return $nodeSecret
}


Write-Host "The Jenkins Secret key for Node: $VmName is: $nodeSecret"

## On the Remote VM/Computer run the PS script that connects the VM/Computer to Jenkins
Invoke-Expression ".\StartJenkinsSlave.ps1 -jenkinsMasterURL $JenkinsUrl -nodeName $VmName -slaveSecret $nodeSecret"

# Close and remove all outstanding Powershell Sessions
Get-PSSession | Remove-PSSession

Write-Host "The VM should now be attached to Jenkins"
return
