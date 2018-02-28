---
layout: default
---

<h1>News</h1>

<ul >
    {% for news in site.categories.news limit:10 %}
    <li>
        <a href="{{news.url}}" class="silent_list">{{news.title}}</a>
        <p>{{news.meta}}</p>
    </li>
    {% endfor %}
</ul>
        
<ul >
    {% for news in site.categories.programming limit:10 %}
    <li>
        <a href="{{news.url}}" class="silent_list">{{news.title}}</a>
        <p>{{news.meta}}</p>
    </li>
    {% endfor %}
</ul>

## All Programming

