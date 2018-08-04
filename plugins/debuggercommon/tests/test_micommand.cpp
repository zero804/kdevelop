/* This file is part of KDevelop
 *
 * Copyright 2018 Friedrich W. H. Kossebau <kossebau@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "test_micommand.h"

// SUT
#include <mi/micommand.h>
// Qt
#include <QTest>

class TestCommandHandler : public QObject, public KDevMI::MI::MICommandHandler
{
    Q_OBJECT
public:
    explicit TestCommandHandler(bool autodelete, int expectedRecordsHandled = -1);
    ~TestCommandHandler() override;
    int recordsHandled() const { return m_recordsHandled; }
public: // MICommandHandler API
    void handle(const KDevMI::MI::ResultRecord& record) override;
    bool autoDelete() override { return m_autodelete; }
private:
    bool m_autodelete;
    int m_expectedRecordsHandled = 0;
    int m_recordsHandled = 0;
};

TestCommandHandler::TestCommandHandler(bool autodelete, int expectedRecordsHandled)
    : m_autodelete(autodelete)
    , m_expectedRecordsHandled(expectedRecordsHandled)
{}

TestCommandHandler::~TestCommandHandler()
{
    QCOMPARE(m_expectedRecordsHandled, m_recordsHandled);
}

void TestCommandHandler::handle(const KDevMI::MI::ResultRecord& record)
{
    QCOMPARE(record.reason, QStringLiteral("reason"));

    ++m_recordsHandled;
}



class TestCommandResultHandler : public QObject
{
    Q_OBJECT
public:
    void handleResult(const KDevMI::MI::ResultRecord& record);
    int recordsHandled() const { return m_recordsHandled; }
private:
    int m_recordsHandled = 0;
};

void TestCommandResultHandler::handleResult(const KDevMI::MI::ResultRecord& record)
{
    QCOMPARE(record.reason, QStringLiteral("reason"));

    ++m_recordsHandled;
}

void TestMICommand::testUserCommand()
{
    KDevMI::MI::UserCommand command(KDevMI::MI::NonMI, "command");
    command.setToken(1);

    // check
    QCOMPARE(command.token(), (uint)1);
    QCOMPARE(command.frame(), -1);
    QCOMPARE(command.thread(), -1);
    QCOMPARE(command.isUserCommand(), true);
    QCOMPARE(command.handlesError(), false);
    QCOMPARE(command.flags(), KDevMI::MI::CommandFlags(KDevMI::MI::CmdMaybeStartsRunning));
    QCOMPARE(command.command(), QStringLiteral("command"));
    QCOMPARE(command.initialString(), QStringLiteral("1command"));
    QCOMPARE(command.cmdToSend(), QStringLiteral("1command\n"));
}

void TestMICommand::testMICommandHandler_data()
{
    QTest::addColumn<bool>("autodelete");

    QTest::newRow("reusable") << false;
    QTest::newRow("burnafterusing") << true;
}

void TestMICommand::testMICommandHandler()
{
    QFETCH(bool, autodelete);

    KDevMI::MI::UserCommand command(KDevMI::MI::NonMI, "command");
    command.setToken(1);

    // set handle and invoke
    QPointer<TestCommandHandler> commandHandler = new TestCommandHandler(autodelete, 1);
    command.setHandler(commandHandler);

    KDevMI::MI::ResultRecord resultRecord1("reason");
    bool success = command.invokeHandler(resultRecord1);

    // check
    QVERIFY(success);
    QCOMPARE(commandHandler.isNull(), autodelete);
    if (!autodelete) {
        QCOMPARE(commandHandler->recordsHandled(), 1);
    }

    // try to invoke again without handler
    // ensure handler no longer exists
    if (!autodelete) {
        delete commandHandler;
    }

    KDevMI::MI::ResultRecord resultRecord2("reason");
    success = command.invokeHandler(resultRecord2);

    // check
    QVERIFY(!success);
}

void TestMICommand::testQObjectCommandHandler()
{
    KDevMI::MI::UserCommand command(KDevMI::MI::NonMI, "command");
    command.setToken(1);

    // set handle and invoke
    auto* resultHandler = new TestCommandResultHandler;
    command.setHandler(resultHandler, &TestCommandResultHandler::handleResult);

    KDevMI::MI::ResultRecord resultRecord1("reason");
    bool success = command.invokeHandler(resultRecord1);

    // check
    QVERIFY(success);
    QCOMPARE(resultHandler->recordsHandled(), 1);

    // delete handler and try to invoke again
    delete resultHandler;

    KDevMI::MI::ResultRecord resultRecord2("reason");
    success = command.invokeHandler(resultRecord2);

    // check
    QVERIFY(!success);
}


QTEST_GUILESS_MAIN(TestMICommand)

#include "test_micommand.moc"
