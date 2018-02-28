---
layout: default
title: Using Makefiles to help with tasks unrelated to programming
meta: Makefiles can be used to perform actions which are unrelated to programming
category: programming
---

# Using 'make' to make life easier
 
## The problem
I was recently asked whether I could create a [makefile](https://www.gnu.org/software/make/) to simplify a *Nix [postfix](http://www.postfix.org/) update process.

> I have a fairly complex postfix setup with multiple files which when you change things you have to execute multiple commands to refresh the configs and start again the postfix function.
> I'd like to have something where you call make and it just does the steps it needs to refresh the config files and then restart postfix
but i can't find a solution that works out of the box without custom scripting a dedicated progam just for that type of 'make' call.

This sounds interesting; challenge accepted.

<br />

First, [`make`](https://www.gnu.org/software/make/) requires 'source' files and output files, the timestamps of which are compared and commands run if necessary. 

Second, how does [postfix](http://www.postfix.org/) work?

To paraphrase:

> Postmap creates a hashed .db file for each of the configuration files 'relay-domains', 'relay-routes'  and so on. You could compare the timestamp of the creation of the .db file against the non-hashed configuration files, and with the exception of the `newaliases` command (global system mail aliases affect /etc/aliases.db based on /etc/aliases) and the `systemctl` call.

Great, given input files, we create output files; [`make`](https://www.gnu.org/software/make/) can handle that.

Recall that [`make`](https://www.gnu.org/software/make/) compares the timestamp of a 'target' with its dependencies on a dependency line.  If one or more of the dependencies are newer than the target, dependency lines are initiated for each dependency until actions are performed.

```makefile
target: dependency1 dependency2 dependency3 dependencyN # Dependeny line
    actions 
```

## The solution

Instead of running one or more of: 

```
postmap /etc/postfix/relay-domains
postmap /etc/postfix/relay-routes
postmap /etc/postfix/virtual_alias
postmap /etc/postfix/canonical
postmap /etc/postfix/sender_canonical
postmap /etc/postfix/recipient_canonical
postmap /etc/postfix/sasl_passwd
postmap /etc/postfix/helo_checks
```

followed by:

```
newaliases
systemctl restart postfix
```

We'll use a [makefile](https://www.gnu.org/software/make/) and streamline the number of commands the user needs to run.

[`make`](https://www.gnu.org/software/make/) searches the contents of the specified directory for the final target, and then it's dependencies for changes.
In this case, there is no single end target as there would be when building an executable in C or C++, but this issue can be circumvented by using a 'phony' target.  It tells [`make`](https://www.gnu.org/software/make/) not to look for that file but allows it to remain as a target.


```makefile
# declare phony target
.phony: run
```

Next, I set up the 'run' target to be dependent upon the configuration files and to run the `newaliases` and `systemctl restart postfix` commands which have to be run when any of the dependencies have changed:

```makefile
run:    relay-domains.db relay-routes.db virtual_alias.db canonical.db sender_canonical.db recipient_canonical.db sasl_passwd.db helo_checks.db 
    newaliases
    systemctl restart postfix
```

Finally, set the next layer of target-dependency information.  Each of the following targets, is a dependency of 'run'.  When 'run' is tested, each of these dependency lines will be tested and the corresponding `postmap` command run if and only if required:

```makefile
relay-domains.db: relay-domains
    postmap /etc/postfix/relay-domains

relay-routes.db: relay-routes
    postmap /etc/postfix/relay-routes

virtual_alias.db: virtual_alias
    postmap /etc/postfix/virtual_alias

canonical.db: canonical
    postmap /etc/postfix/canonical

sender_canonical.db: sender_canonical
    postmap /etc/postfix/sender_canonical

recipient_canonical.db: recipient_canonical
    postmap /etc/postfix/recipient_canonical

sasl_passwd.db: sasl_passwd
    postmap /etc/postfix/sasl_passwd

helo_checks.db: helo_checks
    postmap /etc/postfix/helo_checks
```

Now to process the updates, they only need to run [`sudo make`](https://www.gnu.org/software/make/) from the [postfix](http://www.postfix.org/) directory in order to: 
* update all of the hashed configurations dependency files
* update the aliases
* restart [postfix](http://www.postfix.org/)

It's purely for convenience, but it is making a difference to their workflow.

