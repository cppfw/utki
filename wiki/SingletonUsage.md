# Introduction #

It is often required to create singleton objects in the program, i.e. use the singleton pattern.

**ting** provides a template for quick creation of singleton classes.



<br>
<h1>Header file</h1>
In order to use <i>ting::Singleton</i> one needs to include the following header file:<br>
<pre><code>#include &lt;ting/Singleton.hpp&gt;<br>
</code></pre>



<br>
<h1>Usage</h1>
The usage is simple:<br>
<br>
<pre><code>class MySingletonClass : public ting::Singleton&lt;MySingletonClass&gt;{<br>
public:<br>
    int a;<br>
    int b;<br>
};<br>
<br>
<br>
<br>
int main(int argc, char** argv){<br>
    //create singleton object.<br>
    MySingletonClass mySingletonObject;<br>
<br>
    //NOTE: since this is a singleton class, creating other<br>
    //      instances of this class will cause an exception to be thrown.<br>
<br>
    //access the members of singleton object<br>
    MySingletonClass::Inst().a = 355;<br>
    MySingletonClass::Inst().b = 13;<br>
<br>
    int k = MySingletonClass::Inst().b;<br>
}<br>
</code></pre>

Note, that we create a singleton object inside of the main() function. Though it is not forbidden to create singleton objects in a global scope, it is not recommended and considered a bad practice. This is because when defining objects in global scope and in different files their order of construction is undetermined. Creating objects inside of the main() function allows you to strictly define the order of objects construction.