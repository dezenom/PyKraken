# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

import os
import subprocess

notfound_context = {
    "title": "Page not found",
    "body": """
        <h1>Page not found</h1>
        <p>
            Sorry, we couldn't find that page. It may have been renamed or removed
            in the version of the documentation you're currently browsing.
        </p>
        <p>
            If you're currently browsing the
            <em>latest</em> version of the documentation, try browsing the
            <a href="/en/stable/"><em>stable</em> version of the documentation</a>.
        </p>
        <p>
            Alternatively, use the
            <a href="#" onclick="$('#rtd-search-form [name=\\'q\\']').focus()">Search docs</a>
            box on the left or <a href="/">go to the homepage</a>.
        </p>
    """,
}
project = 'Kraken Engine'
copyright = '2025, Derrick Martinez'
author = 'Derrick Martinez'
release = '0.1.0'
version = '0.1.0'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

needs_sphinx = "7.1"

extensions = [
    "notfound.extension",
    "sphinx.ext.autosectionlabel",
    "sphinx.ext.autodoc",
    "sphinx.ext.intersphinx",
    "sphinx.ext.autosummary",
    "sphinx.ext.napoleon",
]

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

source_suffix = ".rst"
source_encoding = "utf-8-sig"

smartquotes = False

pygments_style = "sphinx"
highlight_language = "python"

master_doc = "index"

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = "sphinx_rtd_theme"
html_theme_options = {
    "logo_only": True,
    "collapse_navigation": False,
}
html_logo = "_static/kraken-engine-banner2.png"
html_static_path = ["_static"]
html_css_files = ["css/custom.css"]
html_js_files = ["js/custom.js"]
html_favicon = '_static/kraken.ico'
html_last_updated_fmt = "%B %d, %Y"
html_title = f"Kraken Engine (v{release}) Documentation"
html_context = {
    "display_github": True,  # Integrate GitHub
    "github_user": "durkisneer1",  # Username
    "github_repo": "Kraken-Engine",  # Repo name
    "github_version": "main",  # Version
    "conf_py_path": "/docs/",  # Path in the checkout to the docs root
    "kraken_docs_title": html_title,
    "kraken_docs_basepath": "https://kraken-engine.readthedocs.io/",
    "kraken_docs_suffix": ".html",
    "kraken_default_lang": "en",
    "kraken_canonical_version": "stable",
    # Set this to `True` when in the `latest` branch to clearly indicate to the reader
    # that they are not reading the `stable` documentation.
    "kraken_is_latest": True,
    "kraken_version": release,
    # Enables a banner that displays the up-to-date status of each article.
    "kraken_show_article_status": True,
}

epub_tocscope = "includehidden"

suppress_warnings = ["autosectionlabel.*"]
