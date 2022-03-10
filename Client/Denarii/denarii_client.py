# A python implementation of the Denarii client. Unlike the C++ and Java versions this one does not allow you
# to directly hash with randomx

import json
import os
import pathlib
import requests
import sys
import workspace_path_finder


def get_home():
    linux_home = ""
    windows_home = ""

    try:
        linux_home = pathlib.Path(os.environ["HOME"])
    except Exception as e:
        print(e)
    try:
        windows_home = pathlib.Path(os.environ["HOMEDRIVE"] + os.environ["HOMEPATH"])
    except Exception as e:
        print(e)

    if os.path.exists(linux_home):
        return linux_home
    else:
        return windows_home


# Append path with the location of all moved python protos
sys.path.append(str(workspace_path_finder.get_home() / "py_proto"))

from Proto import wallet_pb2


class DenariiClient:

    def send_command_to_wallet_rpc(self, method, params):
        """
        Send a json command to the wallet rpc server
        @param method The method to call
        @param params The parameters to call the method with
        @return The json representing the response
        """
        return self.send_command("http://127.0.0.1", "8080", method, params)

    def send_command_to_daemon(self, method, params):
        """
        Send a json command to the daemon rpc server
        @param method The method to call
        @param params The parameters to call the method with
        @return The json representing the response
        """
        return self.send_command("http://127.0.0.1", "8424", method, params)

    def send_command(self, ip, port, method, params):
        """
        Send a command to the specified ip address and port
        @param ip The ip to send to
        @param port The port to use
        @param method The rpc method to call
        @param params The params to pass the rpc method
        @return The json representing the response
        """

        inputs = {
            "method": method,
            "params": params,
            "jsonrpc": "2.0",
            "id": 0,
        }

        res = requests.post(ip + ":" + port + "/json_rpc", data=json.dumps(inputs),
                            headers={"content-type": "application/json"})

        res = res.json()

        return res

    def create_wallet(self, wallet):
        """
        Create a new wallet
        @param wallet The wallet proto with the name and password
        """

        params = {
            "filename": wallet.name,
            "password": wallet.password,
            "language": "English"
        }

        # no output expected
        self.send_command_to_wallet_rpc("create_wallet", params)

    def get_address(self, wallet):
        """
        Get the first address for a wallet
        @param wallet The wallet to store the address in
        """

        params = {
            "account_index": 0
        }

        result = self.send_command_to_wallet_rpc("get_address", params)

        if "result" not in result:
            return

        result_dict = result["result"]

        if "address" in result_dict:
            wallet.address = result_dict["address"]

    def transfer_money(self, amount, sender, receiver):
        """
        Transfer some money between wallets
        @param amount The amount to send
        @param sender The senders wallet
        @param receiver The receivers wallet
        @return The amount sent
        """

        senderCurrentAmount = self.get_balance_of_wallet(sender)

        if senderCurrentAmount < amount:
            return False

        # First set the current wallet to be the sender's wallet
        self.set_current_wallet(sender)

        params = {
            "destinations": [
                {
                    "amount": amount,
                    "addresss": receiver.address
                }
            ]
        }

        res = self.send_command_to_wallet_rpc("transfer", params)

        if "result" not in res:
            return False

        result = res["result"]

        return result["amount"]

    def get_balance_of_wallet(self, wallet):
        """
        Get the balance of a wallet (only does the first address)
        @param wallet the wallet to get balance of
        @return The balance of the wallet
        """

        # First set the current wallet
        self.set_current_wallet(wallet)

        params = {
            "account_index": 0
        }

        res = self.send_command_to_wallet_rpc("get_balance", params)

        if "result" not in res:
            return 0.0

        result = res["result"]

        if "balance" not in result:
            return 0.0

        return result["balance"]

    def set_current_wallet(self, wallet):
        """
        Set the current wallet to be the one passed
        @param wallet the wallet to set as the current one
        """

        params = {
            "filename": wallet.address,
            "password": wallet.password
        }

        self.send_command_to_wallet_rpc("open_wallet", params)

    def get_block_hashing_blob(self, wallet):
        """
        Get the block hashing blob
        @param wallet
        @return true on success and false otherwise
        @return the result json string
        """

        params = {
            "wallet_address": wallet.address
        }

        res = self.send_command_to_wallet_rpc("get_block_template", params)

        if "result" not in res:
            return False, res

        result = res["result"]

        return "blockhashing_blob" in result, result

    def attempt_submit_block(self, mined_block):
        """
        Attempt to submit a mined block
        @param mined_block The string representation of the mined block
        @return True on success and false otherwise
        """

        params = [mined_block]

        res = self.send_command_to_daemon("submit_block", params)

        return "error" not in res
