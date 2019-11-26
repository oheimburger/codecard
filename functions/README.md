![](images/fn.png)

## Introduction

In this guide, we'll show you a few simple steps to launch an Oracle Linux instance on Oracle Cloud Infrastructure, and then proceed to launch your [Fn Project](https://fnproject.io/) Functions server & run your cloud functions. The path that we will take is as follows:

 - Launch your Oracle Linux instance in the Oracle cloud
 - Install Oracle Container Runtime for Docker
 - Install the Fn Functions Server
 - Configure and run your Oracle Code Card Fn functions
 - Create a Fn function for your Code Card

## Create an Oracle Linux instance on the Oracle Cloud Infrastructure
Follow [these instructions](oci.md) to create an Oracle Linux instance and the come back after you are done.

## Configuring your Fn Server

Your Oracle Linux instance is now running, and ready to be configured to host your cloud functions.

### Install and configure Docker

While connected to your Oracle Linux instance, run the following commands to install and configure the Docker container runtime.

```
sudo yum -y install docker-engine-18.03.1.ol-0.0.9.el7.x86_64 -y
sudo usermod -aG docker opc
sudo systemctl enable docker
sudo systemctl start docker
```

Before proceeding further, logout of the current SSH session, & then reconnect to your Oracle Linux instance and log back in vis SSH. _This is to ensure group membership configured in the previous step is correctly applied and in effect._
Once you have reconnected to the instance, run the following command to install the Fn CLI tool (this will download a shell script and execute it).

### Configure Oracle Linux for Fn

```
curl -LSs https://raw.githubusercontent.com/fnproject/cli/master/install | sh
```

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
curl http://localhost:8080/t/codecard/hello
```

or, using the fn client:

```
fn invoke codecard hello
```

or in a browser: http://`linux-instance-public-ip`:8080/t/codecard/hello-trigger

That's it! You just deployed your first function and called it. You are now ready to configure your Code Card to access your cloud function!

## Create a Fn function for your Code Card
The Code Card needs to receive the following JSON format:

Required fields:
```
{
	"template": "template[1-11]",
	"title": "Hello World",
	"subtitle": "This is a subtitle",
	"bodtext": "This is the body",
	"icon": "[see list of named icons| BMP url]",
	"backgroundColor": "[white|black]"
}
```
**Check out the list of available named icons [here](icons.md)*.

Optional fields:
```
{	...
	"badge": [0-100] It will override the icon
	"backgroundImage": "[oracle|codeone | BMP url]" Only for templates that have backgrounds
	"fingerprint": "" The SHA-1 signature of the server containing the custom icon or backgroundImage URL.
	...
}
```
To checkout all available templates go to Oracle Events App -> Code One --> Code Card Designer.

Let's create our first Code Card function!

	fn init --runtime node --trigger http button1
	cd button1

Now lets edit the func.js file using `nano	` or `vi`.

	nano func.js


Modify the handle function to look like this:
```
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
```
In nano `Ctrl` + O and `Ctrl` + X (WriteOut and Exit.)

In vi `ESC`  `:wq` (write and quit.)

Now deploy your new function

	fn deploy --app codecard --local

And test on your browser

	http://`linux-instance-public-ip`:8080/t/codecard/button1

Now you are ready to configure your Code Card to point to your new function! Go to the [Code Card Teminal](https://github.com/cameronsenese/codecard/tree/master/terminal) page to learn how to configure your Card using serial communication.

