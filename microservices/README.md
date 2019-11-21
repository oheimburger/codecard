![](images/helidon.png)

## Introduction

In this guide, we'll show you a few simple steps to launch an Oracle Linux instance on Oracle Cloud Infrastructure, and then proceed to launch your [Fn Project](https://fnproject.io/) Functions server & run your cloud functions. The path that we will take is as follows:

 - Launch your Oracle Linux instance in the Oracle cloud

 - Install Oracle Container Runtime for Docker

 - Install the Fn Functions Server

 - Configure and run your Oracle Code Card Fn functions

 - Create a Fn function for your Code Card

## Prerequisites
 - You will need to have deployed your OKE Kubernetes cluster prior to commencing implementation of the deployment scenario. Follow the link to [this tutorial](https://www.oracle.com/webfolder/technetwork/tutorials/obe/oci/oke-full/index.html) for guidance on the process.
 - Create a 'kube config' authentication artefact. This will be used later in the tutorial to connect to the OKE cluster. Follow the link to [this tutorial](https://www.oracle.com/webfolder/technetwork/tutorials/obe/oci/oke-full/index.html#DownloadthekubeconfigFilefortheCluster) for guidance on the process.
 - Install Docker on the host from which you will be following this work instruction. If you are running Oracle Linux, follow the link to [this tutorial](https://blogs.oracle.com/blogbypuneeth/a-simple-guide-to-docker-installation-on-oracle-linux-75) for guidance on the process.

### Clone the 'codecard/microservices' git repository
Clone the microservices repository:

``` bash
git clone https://github.com/oracle/cloudnative/security/oke-hashicorp-vault-tutorial.git
```

Commands from this point forward will assume that you are in the `../microservices/` directory.

## Create an Oracle Linux instance on the Oracle Cloud Infrastructure


## Configuring your Fn Server



## Install and configure Docker










```
curl -LSs https://raw.githubusercontent.com/fnproject/cli/master/install | sh
​```

At completion, the installation will output the Fn CLI version - per the below example output.

```
fn version 0.5.16

        ______
       / ____/___
      / /_  / __ \
     / __/ / / / /
    /_/   /_/ /_/`

```
#### SELinux constraints
Before you can start Fn you must relax SELinux constraints by running this command:

	sudo setenforce permissive

### Start your Fn Server

Run the following command which will start Fn in the background as a single server mode, using an embedded database and message queue.

	fn start -d

Your Fn server is now instantiated and running in the background.

## Configure and run your Fn functions

Functions are small but powerful blocks of code that generally do one simple thing. Forget about monoliths when using functions, just focus on the task that you want the function to perform. Our CLI tool will help you get started super quickly.

To create a hello world function, run the following command.

	fn init --runtime go --trigger http hello

This will create a simple function in the directory hello, so let's cd into it:

	cd hello

### Deploy your functions to your local Fn server

	fn deploy --app codecard --create-app --local


Now you can call your function locally using curl:

```
curl http://localhost:8080/t/codecard/hello-trigger
​```

or, using the fn client:

```
fn invoke codecard hello
​```

or in a browser: http://`linux-instance-public-ip`:8080/t/codecard/hello-trigger

That's it! You just deployed your first function and called it. You are now ready to configure your Code Card to access your cloud function!

## Create a Fn function for your Code Card
The Code Card needs to receive the following JSON format:

Required fields:
​```
{
	"template": "template[1-11]",
	"title": "Hello World",
	"subtitle": "This is a subtitle",
	"bodtext": "This is the body",
	"icon": "[see list of named icons| BMP url]",
	"backgroundColor": "[white|black]"
}
​```
**Check out the list of available named icons [here](icons.md)*.

Optional fields:
​```
{	...
	"badge": [0-100] It will override the icon
	"backgroundImage": "[oracle|codeone | BMP url]" Only for templates that have backgrounds
	"fingerprint": "" The SHA-1 signature of the server containing the custom icon or backgroundImage URL.
	...
}
​```
To checkout all available templates go to Oracle Events App -> Code One --> Code Card Designer.

Let's create our first Code Card function!

	fn init --runtime node --trigger http button1
	cd button1

Now lets edit the func.js file using `nano	` or `vi`.

	nano func.js


Modify the handle function to look like this:
​```
fdk.handle(function(input){
    let codeCardJson = {
      template: 'template1',
      title: 'Hello there!',
      subtitle: 'How are you?',
      bodytext: 'This is my first Fn function from the Oracle Cloud.',
      icon: 'opensource',
      backgroundColor: 'white'
    }
    return codeCardJson
})
​```
In nano `Ctrl` + O and `Ctrl` + X (WriteOut and Exit.)

In vi `ESC`  `:wq` (write and quit.)

Now deploy your new function

	fn deploy --app codecard --local

And test on your browser

	http://`linux-instance-public-ip`:8080/t/codecard/button1-trigger

Now you are ready to configure your Code Card to point to your new function! Go to the [Code Card Teminal](https://github.com/cameronsenese/codecard/tree/master/terminal) page to learn how to configure your Card using serial communication.