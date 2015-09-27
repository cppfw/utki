# Debugging with ting #

**ting** provides some useful debugging instruments like assertions, tracing and logging.
All the debugging functions are wrapped into macros in order to be able to turn them off in the release builds and turn them on in the debug builds.

For debug builds one needs to define the DEBUG macro. When DEBUG macro is defined ting will do additional checks and assertions which slightly decrease the performance, but for that you will be warned if something goes wrong meaning that there is some bug in the program.
All the ting debug facilities you use in your program will be turned on as well.

For release builds just do not define the DEBUG macro and all debug macros will be turned off.

Note: for g++ compiler macro can be defined by supplying the -DDEBUG key to the g++.

Note: some debug macros are always turned on, regardless of DEBUG macro defined or not. All such macros have `_`ALWAYS suffix in their names.



<br>
<h1>Header file</h1>

In order to use ting debugging facilities, include the following header file:<br>
<pre><code>#include &lt;ting/debug.hpp&gt;<br>
</code></pre>



<br>
<h1>Semicolons after macros</h1>

Note, that semicolons after ting debugging macros are not required. Semicolons are already included inside of the macros. This is because macros can be turned off in release builds, in which case there will be redundant semicolons in your program since macros are evaluated to nothing.<br>
<br>
For example<br>
<pre><code>int a = 35;<br>
<br>
ASSERT(a == 35)  // correct<br>
TRACE(&lt;&lt; "Hello world!" &lt;&lt; std::endl) // correct<br>
<br>
ASSERT(a == 35); // incorrect<br>
TRACE(&lt;&lt; "Hello world!" &lt;&lt; std::endl); // incorrect<br>
</code></pre>



<br>
<h1>ASSERTIONS</h1>

Following assertion macros are available:<br>
<br>
<br>
<br>
<b>ASSERT(x)</b><br>
<b>ASSERT_ALWAYS(x)</b><br>
where <b>x</b> is a boolean expression.<br>
<br>
Example:<br>
<pre><code>int a = 13;<br>
ASSERT(a == 13)<br>
ASSERT(sizeof(int) == 4)<br>
ASSERT(sizeof(a) == 4 &amp;&amp; sizeof(a) == sizeof(int))<br>
</code></pre>


<br>
<b>ASSERT_INFO(x, y)</b><br>
<b>ASSERT_INFO_ALWAYS(x, y)</b><br>
where <b>x</b> is a boolean expression.<br>
<b>y</b> is a logging which needs to be done in case of assertion failure.<br>
<br>
Example:<br>
<pre><code>int a = 13;<br>
ASSERT_INFO(a == 14, "a is not 14, a is " &lt;&lt; a)<br>
</code></pre>
Note, that std::endl is not required as well as leading operator <<.<br>
<br>
<br>
<br>
<b>ASS(x)</b><br>
where <b>x</b> is a <b>pointer</b> or <i>ting::Ref</i>, or <i>ting::Ptr</i>. Generally speaking, <b>x</b> is any value which can be automatically converted to bool-like type and which has operator->() defined.<br>
<br>
This assertion is used to check pointer value for 0 before dereferencing it with <b>operator->()</b>.<br>
<br>
Example:<br>
<pre><code>class TestClass{<br>
public:<br>
    int a;<br>
};<br>
<br>
TestClass *c = new TestClass();<br>
<br>
//make sure, "c" is not 0 before accessing member a.<br>
ASS(c)-&gt;a = 13;<br>
int b = ASS(c)-&gt;a;<br>
</code></pre>

NOTE: that due to implementation specifics, the ASS() macro evaluates the argument two times. So, one should be careful when using the macro for checking, for example, the value returned by function, like this:<br>
<pre><code>bool retVal = ASS(UpdateAndReturnResult());<br>
</code></pre>
Thus, in this case, the UpdateAndReturnResult() function will be called twice when debug mode is enabled (i.e. DEBUG macro is defined).<br>
<br>
<br>
<br>
<b>STATIC_ASSERT(x)</b><br>
where <b>x</b> is a boolean expression which can be evaluated at compile time.<br>
<br>
Static assertion is used to check some conditions in compile time. For example, check size of some class to have a certain value.<br>
STATIC_ASSERT() macro can be used in global scope, inside of a function, inside of a class declaration. It is ok to put STATIC_ASSERT() to template declaration but it will not be checked in that case, i.e. it will not have any effect when using inside of a template.<br>
<br>
Example:<br>
<pre><code>STATIC_ASSERT(sizeof(int) == 4)<br>
<br>
<br>
class TestClass{<br>
public:<br>
    int a;<br>
<br>
    //make sure that unsigned char type and signed char type are 8 bit wide.<br>
    STATIC_ASSERT((unsigned char)(-1) == 0xFF)<br>
    STATIC_ASSERT((signed char)(0xFF) == -1)<br>
    STATIC_ASSERT(sizeof(unsigned char) == sizeof(signed char))<br>
};<br>
<br>
<br>
void MyFunction(TestClass&amp; arg){<br>
    STATIC_ASSERT(sizeof(TestClass) == sizeof(int))<br>
    //...<br>
}<br>
</code></pre>




<br>
<h1>TRACEs and LOGs</h1>

Macros for tracing and logging are available. Tracing macros output info into standard output. Logging macros output info into <i>output.log</i> file.<br>
<br>
Following macros are available:<br>
<br>
<br>
<b>LOG(x)</b><br>
<b>TRACE(x)</b><br>
<b>TRACE_AND_LOG(x)</b><br>
<b>LOG_ALWAYS(x)</b><br>
<b>TRACE_ALWAYS(x)</b><br>
<b>TRACE_AND_LOG_ALWAYS(x)</b><br>
where <b>x</b> is a std::cout like output.<br>
<br>
Example:<br>
<pre><code>LOG(&lt;&lt; "Hello World!" &lt;&lt; std::endl)<br>
<br>
TRACE(&lt;&lt; "sizeof(int) = " &lt;&lt; sizeof(int))<br>
TRACE(&lt;&lt; " and sizeof(short) = " &lt;&lt; sizeof(short) &lt;&lt; std::endl)<br>
<br>
TRACE_AND_LOG(<br>
        &lt;&lt; "Here is some more info about type sizes:" &lt;&lt; std::endl<br>
        &lt;&lt; "sizeof(void*) = " &lt;&lt; sizeof(void) &lt;&lt; std::endl<br>
        &lt;&lt; "sizeof(char) = " &lt;&lt; sizeof(char) &lt;&lt; std::endl<br>
        &lt;&lt; "sizeof(float) = " &lt;&lt; sizeof(float) &lt;&lt; std::endl<br>
        &lt;&lt; "sizeof(double) = " &lt;&lt; sizeof(double) &lt;&lt; std::endl<br>
    )<br>
</code></pre>



<br>
<h1>Debug code</h1>

There is a <b>DEBUG_CODE(x)</b> macro which allows you to define some piece of code which will be present only in debug build and will be excluded from release build (the one created without DEBUG macro defined).<br>
<br>
Example:<br>
<pre><code>DEBUG_CODE(int a = ) DoSomethingAndReturnMinusOneOnFailure();<br>
<br>
ASSERT(a != -1)<br>
</code></pre>

The macro is useful for defining one-line code pieces. Whenever multiple lines of debugging code needs to be defined, one should use<br>
<pre><code>#ifdef DEBUG<br>
<br>
//Do some checks required only in debug builds<br>
//...<br>
<br>
#endif//~DEBUG<br>
</code></pre>



<br>
<h1>Checking the code is executed in correct thread</h1>

In multi-threaded applications, sometimes, there are some constraints regarding in which thread the particular code should be executed. For example, you create some object in one thread and this object requires that its methods would be called from exactly the same thread where it was created. To avoid errors, it is good to put assertions in the methods of that object to check that the method is called from correct thread.<br>
<br>
To achieve that, one can use the <i>ting::Thread::GetCurrentThreadID()</i> static function.<br>
<br>
Example:<br>
<pre><code>//save thread ID<br>
ting::Thread::T_ThreadID threadID = ting::Thread::GetCurrentThreadID();<br>
<br>
//...<br>
<br>
//make sure we are in correct thread<br>
ASSERT(threadID == ting::Thread::GetCurrentThreadID())<br>
</code></pre>