# Debugging with ting #

**utki** provides some useful debugging instruments like assertions, tracing and logging.
All the debugging functions are wrapped into macros in order to be able to turn them off in the release builds and turn them on in the debug builds.

For debug builds one needs to define the DEBUG macro. When DEBUG macro is defined ting will do additional checks and assertions which slightly decrease the performance, but for that you will be warned if something goes wrong meaning that there is some bug in the program.
All the ting debug facilities you use in your program will be turned on as well.

For release builds just do not define the DEBUG macro and all debug macros will be turned off.

Note: for g++ compiler macro can be defined by supplying the -DDEBUG key to the g++.

Note: some debug macros are always turned on, regardless of DEBUG macro defined or not. All such macros have `_`ALWAYS suffix in their names.



<br>
<h1>Header file</h1>

In order to use ting debugging facilities, include the following header file:<br>
<pre><code>#include &lt;utki/debug.hpp&gt;<br>
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




<br>
<h1>TRACEs</h1>

Macros for tracing (logging) are available. Tracing macros output info into standard output.<br>
<br>
Following macros are available:<br>
<br>
<br>
<b>TRACE(x)</b><br>
<b>TRACE_ALWAYS(x)</b><br>
where <b>x</b> is a std::cout like output.<br>
<br>
Example:<br>
<pre><code>
TRACE(&lt;&lt; "sizeof(int) = " &lt;&lt; sizeof(int))<br>
TRACE(&lt;&lt; " and sizeof(short) = " &lt;&lt; sizeof(short) &lt;&lt; std::endl)<br>
<br>
</code></pre>
