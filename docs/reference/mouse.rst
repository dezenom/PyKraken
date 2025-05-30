mouse
=====

Description
-----------

The **mouse** namespace contains functions to handle input from the mouse.

Usage
-----

.. raw:: html

    <div class="highlight"><pre>
    <span class="c1"># Check if the left mouse button is currently pressed</span>
    <span class="k">if</span> <span class="n">kn</span><span class="o">.</span><span class="n">mouse</span><span class="o">.</span><span class="nf">is_pressed</span><span class="p">(</span><span class="n">kn</span><span class="o">.</span><span class="nc">M_LEFT</span><span class="p">):</span>
        <span class="nf">print</span><span class="p">(</span><span class="s">"Left button is pressed"</span><span class="p">)</span>

    <span class="c1"># Get the current mouse position and draw a circle at that position</span>
    <span class="n">pos</span> <span class="o">=</span> <span class="n">kn</span><span class="o">.</span><span class="n">mouse</span><span class="o">.</span><span class="nf">get_pos</span><span class="p">()</span>
    <span class="n">kn</span><span class="o">.</span><span class="n">draw</span><span class="o">.</span><span class="nf">circle</span><span class="p">(</span><span class="n">pos</span><span class="p">,</span> <span class="mi">10</span><span class="p">,</span> <span class="n">kn</span><span class="o">.</span><span class="n">color</span><span class="o">.</span><span class="nc">RED</span><span class="p">)</span>
    </pre></div>

Functions
---------

.. autofunction:: pykraken.mouse.is_pressed(button)

.. autofunction:: pykraken.mouse.is_just_pressed(button)

.. autofunction:: pykraken.mouse.is_just_released(button)

.. autofunction:: pykraken.mouse.get_pos()

.. autofunction:: pykraken.mouse.get_rel()

.. autofunction:: pykraken.mouse.lock()

.. autofunction:: pykraken.mouse.unlock()

.. autofunction:: pykraken.mouse.is_locked()

.. autofunction:: pykraken.mouse.hide()

.. autofunction:: pykraken.mouse.show()

.. autofunction:: pykraken.mouse.is_hidden()
