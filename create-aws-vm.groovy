import hudson.model.*
import jenkins.model.*
import groovy.json.JsonSlurper

ipAddress="0.0.0.0"

class PipelineUtilities extends AbstractPipeline {
    def scriptsPathRelative
    PipelineUtilities(script) {
        super(script)
        scriptsPathRelative = "src\\agilent\\sid\\cicd\\scripts\\ps"
    }
    def runPowerShell(psCmd, args, debug=true)
    {
        def output
        if (debug) script.println "[DEBUG] Powershell script: \n $psCmd"
        if (debug) script.println "[DEBUG] Arguments to powershell script: \n $args"

        try
        {
            def shellCommand = "${psCmd} ${args}"
            shellCommand = ResolveEnvVars(shellCommand)
            if(debug) script.println "[DEBUG] The following command will be executed: \n ${shellCommand}"
            output = script.powershell """${shellCommand}"""
        } catch (Exception e) {
            script.println "PipelineUtilities.runPowerShell() - caught an exception"
            script.error e.toString()
        }
        return output
    }

    //Function to resolve any environment variables in the string
    def ResolveEnvVars(String text)
    {
        script.println "Text to be resolved: " + text
        //Script that expands any existing environment variables with actual values.
        def resolvedText = script.powershell(returnStdout:true, script: """${script.env.WORKSPACE}\\${scriptsPathRelative}\\ResolveEnvVars.ps1 \"${text}\"""")
        script.println "Resolved Text: " + resolvedText
        return resolvedText
    }

    //Get current branch name
    def getBranchName() {
        def branchName = script.scm.branches[0].name.split('/').last() //Extract only branch name excluding 'origin'
        
        script.println "Branch Name: $branchName"
        return branchName        
    }

    // Checkout specified git branch from specified git repository 
    void checkoutGitBranch(String branch, String repo='ssh://git@sparksource.collaboration.agilent.com:7999/sid/jenkinsscripts.git')
    {
        script.println "Checking out source from Repository: $repo, Branch: $branch"
        script.git credentialsId: 'a4f7851b-5e64-4301-a5cc-86eb9d8856be', branch: "$branch", url: "$repo"
    }

    //Stop antivirus to speed up copy.
    void stopSymantecEP()
    {
        script.println "Stopping Symatec Endpoint Protection to speed up the file copy/download"
        runPowerShell("Start-Process", "'%ProgramFiles(x86)%\\Symantec\\Symantec Endpoint Protection\\smc' -ArgumentList -stop -Verb runAs")
    }

    //Download specified artifact from Nexus.
    void downloadCdsPkg(version, destDir, expand=false)
    {
        script.println "Downloading CDS Installer version: ${version} from Nexus"
        // Default values for 2.5 release
        def archiveName = "CDS-${version}-Release.zip"
        def nexusUrl = "http://scsgitsrv01.scs.agilent.com:8081/nexus/content/repositories/sprintreleases/Agilent/OpenLab/Installer/Packaging/CDS/${version}/${archiveName}"
        if( version.startsWith('2.4') )
        {
            //Reset values for 2.4
            archiveName = "OpenLab-${version}-CDS.zip"
            nexusUrl = "http://scsgitsrv01.scs.agilent.com:8081/nexus/service/local/repositories/releases/content/Agilent/OpenLab/Installer/OpenLab/${version}/${archiveName}"
        }
        
        if( version.startsWith('2.3'))
        {            
            //Reset values for 2.3
            archiveName = "OpenLabCDS-${version}.zip"
            nexusUrl = "http://scsgitsrv01.scs.agilent.com:8081/nexus/service/local/repositories/releases/content/Agilent/OpenLab/Installation/CDSInstaller/OpenLabCDS/${version}/${archiveName}"
        }
        
        if( version.startsWith('2.2') )
        {
            //Reset values for 2.2
            archiveName = "OpenLABCDS-${version}.zip"
            nexusUrl = "http://scsgitsrv01.scs.agilent.com:8081/nexus/service/local/repositories/releases/content/Agilent/OpenLAB/Installation/CDSInstaller/OpenLABCDS/${version}/${archiveName}"
        }
        
        //make sure deploy path is created before downloading build
        script.dir(destDir)
        {
            runPowerShell ("${script.env.WORKSPACE}\\${scriptsPathRelative}\\DownloadFile.ps1", "-URL ${nexusUrl} -FullPath ${destDir}\\${archiveName}")
        }
        //extract to same folder if caller want it
        if (expand) expandArchive("${destDir}\\${archiveName}", destDir)
    }

    //Extract contents of an archive. Extract to same directory as archive if destination is not specified
    void expandArchive(archive, destDir)
    {
        script.println "Expanding archive: ${archive} to: ${destDir}"
        //Extract from within the directory
        script.dir(destDir)
        {
            runPowerShell ("${script.env.WORKSPACE}\\${scriptsPathRelative}\\ExpandArchive.ps1", "-ArchivePath ${archive} -ExtractTo ${destDir}")
        }
    }

    //Install CDS Workstaion - File System.
    void installCdsWs(pkgLocation, filesystem=false, secure=false, configFilePath="")
    {
        script.println "Installing CDS from: ${pkgLocation}, Configuration file location: ${configFilePath}"
        script.println "File System Installation: " + filesystem
        script.println "Secured: " + secure
        script.println "Content Management Installation: " + !filesystem

        def fileConfig = new File(configFilePath)
        if( fileConfig.exists() ) runPowerShell ("${script.env.WORKSPACE}\\${scriptsPathRelative}\\InstallCds.ps1", "-Installer '${pkgLocation}\\Setup\\CDSInstaller.exe' -ConfigFile '${configFilePath}' -InstallProperties 'SecureFS=${secure}'")
        else { 
            if(filesystem) 
                runPowerShell ("${script.env.WORKSPACE}\\${scriptsPathRelative}\\InstallCds.ps1", "-Installer '${pkgLocation}\\Setup\\CDSInstaller.exe' -InstallProperties 'LicenseAccepted=True SecureWorkstation=True StorageFileSystem=True SecureFS=${secure}'")
            else
                runPowerShell ("${script.env.WORKSPACE}\\${scriptsPathRelative}\\InstallCds.ps1", "-Installer '${pkgLocation}\\Setup\\CDSInstaller.exe' -InstallProperties 'LicenseAccepted=True SecureWorkstation=True StorageDataStore=True OlssAdminPassword=plain-password:admin DbAdminPassword=plain-password:postgres'")
        }
    }

    //Kill all running/blocked/Pending builds for current JOB.
    void killAllBuilds()
    {
        while(script.currentBuild.rawBuild.getPreviousBuildInProgress() != null)
        {
            script.println "Killing Build: " + script.currentBuild.rawBuild.getPreviousBuildInProgress()
            script.currentBuild.rawBuild.getPreviousBuildInProgress().doKill()
        }
    }

    //Get Jankins master base URL from build URL.
    def getJenkinsMasterBaseURL(buildURL)
    {
        URI uri = new URI(buildURL)
        def port = uri.getPort()
        def baseUrl = uri.getScheme() + "://" + uri.getHost()

        //Returns http(s)://[host OR IP]:[port]
        if(port != -1) //If port is part of URL
            baseUrl = baseUrl + ":" + port

        script.println "Jenkins Master Base URL: " + baseUrl
        return baseUrl
    }

    //Get Jankins slave node's secret.
    def getJenkinsSlaveSecret(slaveNode)
    {
        //Returns specified slave node's secret
        def secret = Jenkins.getInstance().getComputer(slaveNode).getJnlpMac()
        script.println "Jenkins Slave Node secret: " + secret
        return secret
    }

    //Run Jankins slave on node.
    void startJenkinsSlave(masterURL, slaveNode)
    {
        //Get slave node's secret
        def secret = getJenkinsSlaveSecret(slaveNode)
        //run slave on remote node
        runPowerShell("${script.env.WORKSPACE}\\${scriptsPathRelative}\\StartJenkinsSlave.ps1", "-jenkinsMasterURL ${masterURL} -nodeName ${slaveNode} -slaveSecret ${secret}")
    }
}

// Abstract Class that implements Serializable and consumes script object to access pipeline steps and properties
abstract class AbstractPipeline implements Serializable {

  def script

  AbstractPipeline ( script ) {
    this.script = script
  }
}
//----------------------------------
node {
    def utils = new PipelineUtilities(this)
    def publicDNS = ""
    parameters {
        string(name: 'KEY_PAIR_NAME', defaultValue: "", description: 'The name of the key pair created in AWS.')
        string(name: 'INSTANCE_NAME', defaultValue: "", description: 'The name of the instance that will be created. This will be the host name of the instance, so no spaces...')
        string(name: 'AMI', defaultValue: "", description: 'The Amazon Machine Image (AMI) ID used to specify the OS to run on the instance.')
    }
    try {
        echo """
        Parameter KEY_PAIR_NAME is: ...................... ${KEY_PAIR_NAME}
        Parameter INSTANCE_NAME is: ...................... ${INSTANCE_NAME}
        Parameter AMI is: ................................ ${AMI}
        """

        stage('Launching instance') {
            def tags = "ResourceType=instance,Tags=[{Key=Name,Value=${INSTANCE_NAME}}]"
            def proc = "aws ec2 run-instances --image-id ${AMI} --count 1 --instance-type t2.micro --key-name ${KEY_PAIR_NAME} --tag-specifications ${tags}".execute()

            proc.waitFor()
            def result = proc.text
            println result

            def jsonParser = new JsonSlurper()
            def instanceID = jsonParser.parseText(result).Instances.InstanceId.get(0)

            println "instanceID ${instanceID}"

            // sleep(5) // Wait for public DNS to be assigned

            def test = "Reservations[].Instances[].PublicDnsName"
            def proc2 = "aws ec2 describe-instances --instance-id ${instanceID} --query ${test}".execute()
            proc2.waitFor()
            println proc2.text
            //publicDNS = proc2.text
            
            //println "publicDNS ${publicDNS}"
        }

    } catch (ex) {
        echo 'Creating instance failed'
        currentBuild.result = 'FAILURE'
        throw ex
    } finally {
        echo 'Status Of VM Creation...'
        def currentResult = currentBuild.result ?: 'SUCCESS'
        def previousResult = currentBuild.previousBuild?.result
        def title = "${currentResult}: Job '${env.JOB_NAME} [${env.BUILD_NUMBER}]'"
        def message
        // Override default values based on build status
        // Success or failure, always send notifications
        if (currentResult == 'FAILURE') {
            message = """<p>${currentResult}: Job '${env.JOB_NAME} [${env.BUILD_NUMBER}]'</p>
            <p>Failed to create VM: ${INSTANCE_NAME}, check console output at "<a href="${env.BUILD_URL}">${env.JOB_NAME} [${env.BUILD_NUMBER}]</a>"</p>"""
        } 
        if (currentResult == 'UNSTABLE') {
            echo 'Build is Unstable'
        }
        if (previousResult != null && previousResult != currentResult) {
            echo 'Build status changed from last build'
        }
        if (currentResult == 'SUCCESS') {
            echo 'Successfully created instance'
            message = """<p>***** VM created successfully *****</p>
            <p>VM Name: ${INSTANCE_NAME}</p><p>IP Address: ${ipAddress}</p>
            <p>Remote Desktop Connection: <a href="mstsc.exe /v:${ipAddress}">Connect to ${INSTANCE_NAME}</a></p>
            <p>NOTE: Outlook does not let any program run through links. So to launch remote desktop, right click on the link, copy and paste it on command prompt or windows run [Windows button + r] command, then hit 'Enter'</p>"""
        }
        // Send email notifications to the users who started the build.
        //emailext (subject: title, body: message, recipientProviders: [[$class: 'RequesterRecipientProvider']] )
    }
}
return this
