class TestCoverage:
  coverage = [False] * 10000
import re

class Sample1:

    test_cert = 7375371

    valid_hosts = []

    def _GetValidHostsForCert(self, cert):
        assert cert == self.test_cert
        return self.valid_hosts

#START OF SAMPLE
    def _ValidateCertificateHostname(self, cert, hostname):
        hosts = self._GetValidHostsForCert(cert)
        for host in hosts:
            host_re = host.replace('.', '\.').replace('*', '[^.]*')
            if re.search('^%s$' % (host_re,), hostname, re.I):
                return True
        return False
#END OF SAMPLE


s = Sample1()

s.valid_hosts = []

assert False == s._ValidateCertificateHostname(s.test_cert, "")
assert False == s._ValidateCertificateHostname(s.test_cert, ".")

s.valid_hosts = ["a.b"]

assert False == s._ValidateCertificateHostname(s.test_cert, "a")
assert False == s._ValidateCertificateHostname(s.test_cert, "a,b")
assert True == s._ValidateCertificateHostname(s.test_cert, "a.b")
assert True == s._ValidateCertificateHostname(s.test_cert, "A.B")
assert False == s._ValidateCertificateHostname(s.test_cert, "a.b ")
assert False == s._ValidateCertificateHostname(s.test_cert, "zzz")

s.valid_hosts = ["a.b", "zzz"]

assert False == s._ValidateCertificateHostname(s.test_cert, "a")
assert False == s._ValidateCertificateHostname(s.test_cert, "a,b")
assert True == s._ValidateCertificateHostname(s.test_cert, "a.b")
assert True == s._ValidateCertificateHostname(s.test_cert, "A.B")
assert False == s._ValidateCertificateHostname(s.test_cert, "a.b ")
assert True == s._ValidateCertificateHostname(s.test_cert, "zzz")

s.valid_hosts = ["a*b"]

assert True == s._ValidateCertificateHostname(s.test_cert, "ab")
assert True == s._ValidateCertificateHostname(s.test_cert, "aaaaab")
assert True == s._ValidateCertificateHostname(s.test_cert, "abbbb")
assert True == s._ValidateCertificateHostname(s.test_cert, "AouauB")
assert False == s._ValidateCertificateHostname(s.test_cert, "aba")
assert False == s._ValidateCertificateHostname(s.test_cert, "bab")
assert False == s._ValidateCertificateHostname(s.test_cert, "a.b")
