
[![GitHub license](https://img.shields.io/badge/license-apache-blue.svg?style=flat)](https://www.apache.org/licenses/LICENSE-2.0)
[![Build Status](https://travis-ci.org/jarulraj/sqlcheck.svg?branch=master)](https://travis-ci.org/jarulraj/sqlcheck)
[![Coverage Status](https://coveralls.io/repos/github/jarulraj/sqlcheck/badge.svg?branch=master)](https://coveralls.io/github/jarulraj/sqlcheck?branch=master)

## What Is SQLCheck?

SQLCheck is a tool for detecting SQL anti-patterns. The primary goal of SQLCheck is to automate the detection of common SQL anti-patterns that often end up slowing down  SQL queries.

## Installation

SQLCheck requires the following tools:

- **g++ 4.9+** 
- **cmake** (`apt-get install cmake`) 

Run the following commands to install SQLCheck:

```
./bootstrap
cd build
cmake -DCMAKE_BUILD_TYPE=RELEASE ..
make
```

## What it can do
Right now Gixy can find:
  * [[ssrf] Server Side Request Forgery](https://github.com/yandex/gixy/blob/master/docs/en/plugins/ssrf.md)
  * [[http_splitting] HTTP Splitting](https://github.com/yandex/gixy/blob/master/docs/en/plugins/httpsplitting.md)
  * [[origins] Problems with referrer/origin validation](https://github.com/yandex/gixy/blob/master/docs/en/plugins/origins.md)
  * [[add_header_redefinition] Redefining of response headers by  "add_header" directive](https://github.com/yandex/gixy/blob/master/docs/en/plugins/addheaderredefinition.md)
  * [[host_spoofing] Request's Host header forgery](https://github.com/yandex/gixy/blob/master/docs/en/plugins/hostspoofing.md)
  * [[valid_referers] none in valid_referers](https://github.com/yandex/gixy/blob/master/docs/en/plugins/validreferers.md)
  * [[add_header_multiline] Multiline response headers](https://github.com/yandex/gixy/blob/master/docs/en/plugins/addheadermultiline.md)

You can find things that Gixy is learning to detect at [Issues labeled with "new plugin"](https://github.com/yandex/gixy/issues?q=is%3Aissue+is%3Aopen+label%3A%22new+plugin%22)


# Usage
By default Gixy will try to analyze Nginx configuration placed in `/etc/nginx/nginx.conf`.

But you can always specify needed path:
```
$ gixy /etc/nginx/nginx.conf

==================== Results ===================

Problem: [http_splitting] Possible HTTP-Splitting vulnerability.
Description: Using variables that can contain "\n" may lead to http injection.
Additional info: https://github.com/yandex/gixy/blob/master/docs/ru/plugins/httpsplitting.md
Reason: At least variable "$action" can contain "\n"
Pseudo config:
include /etc/nginx/sites/default.conf;

	server {

		location ~ /v1/((?<action>[^.]*)\.json)?$ {
			add_header X-Action $action;
		}
	}


==================== Summary ===================
Total issues:
    Unspecified: 0
    Low: 0
    Medium: 0
    High: 1
```


## Contributions

Contributions to SQLCheck are always welcome. You can contribute in different ways:
* Open an issue with suggestions for improvements and errors you're facing;
* Fork this repository and submit a pull request;
* Improve the documentation.

## License

Copyright (c) 2017 [CMU Database Group](http://db.cs.cmu.edu/)  
Licensed under the [Apache License](LICENSE).
