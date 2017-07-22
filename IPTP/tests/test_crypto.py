import unittest
from crypto import ntrubinding

class TestNTRU(unittest.TestCase):

    def setUp(self):
        self.data = b"blabla"
        self.keys = ntrubinding.gen_keypair()
        self.encrypted = ntrubinding.encrypt(self.keys['pubkey'], self.data)

    def test_decryption(self):
        self.assertEqual(self.data, ntrubinding.decrypt(self.keys['pubkey'], self.keys['privkey'], self.encrypted))

if __name__ == '__main__':
    unittest.main()
