# gridlab-d-federates
This repository contains the model and business logic code for market and demand controllers which interact with a gridlab D simulation and its associated federate.

## Updating for Java Development

```bash
sudo apt-get install openjdk-7-jdk ant
```

## Installing Gridlab-D

You'll need to install the prerequisites for building gridlab-d:

```bash
sudo apt-get install automake autoconf libtool curl
```

Then you can checkout the source code for gridlab-d from the SVN repository:

```bash
svn co svn://svn.code.sf.net/p/gridlab-d/code/trunk gridlab-d-code
```

Then build and install gridlab-d on your system:

```baash
cd gridlab-d-code
autoreconf -isf
./configure
make
sudo make install
```

## Setting up the GLD Federates example

Make the `GridlabDHelloWorld` example folder in `$C2WTROOT`:

```bash
mkdir -p $C2WTROOT/examples/GridlabDHelloWorld
```

Copy the model files to their respective locations:

```bash
cp -r models $C2WTROOT/examples/GridlabDHelloWorld/.
```

Open the copied XME with GME and run the `C2W Main Interpreter` and the `C2W Deployment Interpreter`.

Copy `cpp` to `$C2WTROOT/examples/GridlabDHelloWorld`

```bash
cp -r cpp $C2WTROOT/examples/GridlabDHelloWorld/.
```

### Build the copied code:

```bash
cd $C2WTROOT/examples/GridlabDHelloWorld/cpp
cmake .
make
```

### Create the build targets for the generated java code

  * Edit the file `$C2WTROOT/build.properties`

    * Add the following properties under the `helloworld` properties:

    ```bash
    build.java.gridlabdhelloworld.dir = ${build.java.core.dir}
    src.generated.gridlabdhelloworld.dir = ${src.generated.dir}/GridlabDHelloWorld
    src.generated.gridlabdhelloworld.java.dir = ${src.generated.gridlabdhelloworld.dir}/java
    ```

  * Edit the file `$C2WTROOT/build.xml`

    * add the following target under the `compile-HelloWorld` target:

    ```xml
    <target name="compile-GridlabDHelloWorld" depends="compile-core">
            <javac source="1.6" target="1.6" destdir="${build.java.gridlabdhelloworld.dir}" debug="on" debuglevel="lines,vars,source" includejavaruntime="yes" includeantruntime="yes" failonerror="false" nowarn="true">
                    <!--<compilerarg value="-verbose"/>
                    <compilerarg value="-Xlint:all"/>-->

                    <src path="${src.generated.gridlabdhelloworld.java.dir}"/>
                    <classpath refid="c2wt.class.path"/>
            </javac>
    </target>
    ```

    * Add `compile-GridlabDHelloWorld` to the `depends` list of the the target `compile`

### Build the generated java code

Compile the code by running `ant` from `$C2WTROOT` or by opening the
Omnet++ IDE and running `c2wt build.xml` option from the run custom
command toolbar button menu.

### Prepare the Scripts

Make the generated script executable:

```bash
chmod +x $C2WTROOT/generated/GridlabDHelloWorld/scripts/main-Deployment/Main/start.sh
```

## Run the Federates

```bash
$C2WTROOT/generated/GridlabDHelloWorld/scripts/main-Deployment/Main/start.sh
```
